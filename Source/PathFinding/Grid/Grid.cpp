// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "GridTile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PathFinding/Controller/PathFindingPlayerController.h"
#include "PathFinding/Character/PathfindingCharacter.h"
#include "Containers/Array.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(SceneRoot);

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();

	SpawnTile(false);
	PlayerController = Cast<APathFindingPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void AGrid::SpawnTile(bool SpawnNoneTile)
{
	GridLocation = GetActorLocation();
	GenerateMapDataFromWorld();
	for (auto& Data : PathData) {
		if (PathData.Find(Data.Key)->GroundTypes != GroundType::None || SpawnNoneTile) {
			AGridTile* Tile = GetWorld()->SpawnActor<AGridTile>(GridTile, PathData.Find(Data.Key)->WorldLocation, FRotator::ZeroRotator);
			PathData.Find(Data.Key)->TileActor = Tile;
			Tile->GridIndex = Data.Key;
			Tile->Grid = this;
		}
	}
}

void AGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrid::DrawTile()
{
	for (auto& Data : PathData) {
		GridDrawPlane(PathData.Find(Data.Key)->WorldLocation, PathData.Find(Data.Key)->GroundTypes);
	}
}

void AGrid::GenerateMapDataFromWorld()
{
	int32 X;
	int32 Y;

	for (int32 x = 0; x < GridTileNumber().X; x++)
	{
		X = x;
		for (int32 y = 0; y < GridTileNumber().Y; y++)
		{
			Y = y;
			float MultipleX = (X * (TileSize * 2)) + TileSize;
			float MultipleY = (Y * (TileSize * 2)) + TileSize;
			FVector TilePos = GridBottomLeft() + (GetActorRightVector() * MultipleX) + (GetActorForwardVector() * MultipleY);
			FHitResult ResultHitGround;
			bool IsHitGround = GridSpheretrace(TilePos, ETraceTypeQuery::TraceTypeQuery3, ResultHitGround);
			if (IsHitGround) {
				FHitResult ResultHitObstacle;
				bool IsHitObstacle = GridSpheretrace(TilePos, ETraceTypeQuery::TraceTypeQuery4, ResultHitObstacle);
				if (IsHitObstacle) {
					FPathFindingData NewPath;
					NewPath.GridIndex = UKismetMathLibrary::MakeVector2D(X, Y);
					NewPath.GroundTypes = Cast<ABaseObstacle>(ResultHitObstacle.GetActor())->GroundTypes;
					NewPath.WorldLocation = TilePos;
					NewPath.TileCost = GetTileCost(Cast<ABaseObstacle>(ResultHitObstacle.GetActor())->GroundTypes);
					PathData.Add(NewPath.GridIndex, NewPath);
				}
				else {
					FPathFindingData NewPath;
					NewPath.GridIndex = UKismetMathLibrary::MakeVector2D(X, Y);
					NewPath.GroundTypes = GroundType::Normal;
					NewPath.WorldLocation = TilePos;
					NewPath.TileCost = GetTileCost(GroundType::Normal);
					PathData.Add(NewPath.GridIndex, NewPath);
				}
			}
			else {
				FPathFindingData NewPath;
				NewPath.GridIndex = UKismetMathLibrary::MakeVector2D(X, Y);
				NewPath.GroundTypes = GroundType::None;
				NewPath.WorldLocation = TilePos;
				NewPath.TileCost = GetTileCost(GroundType::None);
				PathData.Add(NewPath.GridIndex, NewPath);
			}
		}
	}
}

void AGrid::FindPathToTarget(FVector2D StartTile, FVector2D TargetTile)
{
	FVector2D startTile = StartTile;
	FVector2D targetTile = TargetTile;

	Path.Empty();

	GroundType typeGround = PathData.Find(targetTile)->GroundTypes;
	if (PathData.Find(targetTile) && typeGround != GroundType::None && typeGround != GroundType::Impossible) {

		//Default Value
		TMap<FVector2D, FPathFindingData> NewPathData;
		for (auto& pathData : PathData) {
				FPathFindingData newData0;
				newData0.TileActor = PathData.Find(pathData.Key)->TileActor;
				newData0.UnitOnThisTile = PathData.Find(pathData.Key)->UnitOnThisTile;
				newData0.GroundTypes = PathData.Find(pathData.Key)->GroundTypes;
				newData0.WorldLocation = PathData.Find(pathData.Key)->WorldLocation;
				newData0.GridIndex = PathData.Find(pathData.Key)->GridIndex;
				newData0.TileCost = PathData.Find(pathData.Key)->TileCost;
				newData0.FinalCost = 999;
				newData0.CostFromStart = 999;
				newData0.EstimatedCostToTarget = 999;

				NewPathData.Add(pathData.Key, newData0);
		}
		for (auto& data : NewPathData) {
			PathData.Add(data);
		}

		AddNewPathData(startTile, targetTile, 0, PathData.Find(startTile)->PreviousTile);

		TArray<FVector2D> openSet;
		TArray<FVector2D> closedSet;
		FVector2D currentTile = startTile;
		FVector2D openSetCheapest;
		openSet.AddUnique(currentTile);
		while (openSet.Num() > 0)
		{
			openSetCheapest = openSet[0];
			for (auto& open : openSet) {
				FPathFindingData* openSetData = PathData.Find(open);
				FPathFindingData* openSetCheapestData = PathData.Find(open);
				if (openSetData->FinalCost < openSetCheapestData->FinalCost) {
					openSetCheapest = open;
				}
				else {
					if (openSetData->FinalCost == openSetCheapestData->FinalCost) {
						if (openSetData->EstimatedCostToTarget < openSetCheapestData->EstimatedCostToTarget) {
							openSetCheapest = open;
						}
					}
				}
			}
			currentTile = openSetCheapest;
			openSet.Remove(currentTile);
			closedSet.AddUnique(currentTile);

			FVector2D currentNeighbour;
			int32 CurrentNeighbourCostFromStart;
			for (auto& tileNeighbours : GetTileNeighbours(currentTile)) {
				currentNeighbour = tileNeighbours;
				if (!closedSet.Contains(currentNeighbour)) {
					FPathFindingData* currentTileData = PathData.Find(currentTile);
					FPathFindingData* currentNeighbourstData = PathData.Find(currentNeighbour);
					CurrentNeighbourCostFromStart = currentTileData->CostFromStart + currentNeighbourstData->TileCost;
					if (!openSet.Contains(currentNeighbour)) {
						openSet.AddUnique(currentNeighbour);

						AddNewPathData(currentNeighbour, targetTile, CurrentNeighbourCostFromStart, currentTile);

						if (currentNeighbour == targetTile) {
							Path = RetracePath(targetTile, startTile);
						}
					}
					else {
						if (CurrentNeighbourCostFromStart < currentNeighbourstData->CostFromStart) {
							AddNewPathData(currentNeighbour, targetTile, CurrentNeighbourCostFromStart, currentTile);

							if (currentNeighbour == targetTile) {
								Path = RetracePath(targetTile, startTile);
							}
						}
					}
				}
			}
		}
	}
}

void AGrid::AddNewPathData(FVector2D& currentNeighbour, const FVector2D& targetTile, const int32& CurrentNeighbourCostFromStart, const FVector2D& currentTile)
{
	FPathFindingData newData;
	newData.TileActor = PathData.Find(currentNeighbour)->TileActor;
	newData.UnitOnThisTile = PathData.Find(currentNeighbour)->UnitOnThisTile;
	newData.GroundTypes = PathData.Find(currentNeighbour)->GroundTypes;
	newData.WorldLocation = PathData.Find(currentNeighbour)->WorldLocation;
	newData.GridIndex = PathData.Find(currentNeighbour)->GridIndex;
	newData.TileCost = PathData.Find(currentNeighbour)->TileCost;
	newData.FinalCost = GetEstimatedCostToTarget(currentNeighbour, targetTile) + CurrentNeighbourCostFromStart;
	newData.CostFromStart = CurrentNeighbourCostFromStart;
	newData.EstimatedCostToTarget = GetEstimatedCostToTarget(currentNeighbour, targetTile);
	newData.PreviousTile = currentTile;
	PathData.Add(currentNeighbour, newData);
}

void AGrid::SetSelectedTile(AGridTile* NewTileSelected)
{
	if (IsValid(SelectedTile)) {
		SelectedTile->SetSelected(false);
	}
	SelectedTile = NewTileSelected;

	if(!bIsUnitMoving)
		PlayerController->NewPathAvaible(Path);
}

void AGrid::SetOveredTile(AGridTile* NewOveredTile)
{
	if (IsValid(OveredTile)) {
		OveredTile->SetOvered(false);
		HighlightCurrenPath(false);
		if (IsValid(PlayerController->SelectedUnit) && IsValid(OveredTile) && !bIsUnitMoving) {
			FindPathToTarget(PlayerController->SelectedUnit->GridIndex, OveredTile->GridIndex);
			HighlightCurrenPath(true);
		}
	}
	OveredTile = NewOveredTile;
}

void AGrid::HighlightCurrenPath(bool bIsForHighlight)
{
	for (auto& path : Path) {
		if (PathData.Find(path)->TileActor != nullptr) {
			PathData.Find(path)->TileActor->IsInPath(bIsForHighlight);
		}
	}
}

void AGrid::GridDrawPlane(FVector& TilePos, TEnumAsByte<GroundType> TypeGround)
{
	FLinearColor LinearColor;
	switch (TypeGround)
	{
	case Normal:
		LinearColor = FLinearColor::White;
		break;
	case Diffictuly:
		LinearColor = FLinearColor::Blue;
		break;
	case ReallyDiffictuly:
		LinearColor = FLinearColor::Yellow;
		break;
	case Impossible:
		LinearColor = FLinearColor::Red;
		break;
	default:
		break;
	}
	UKismetSystemLibrary::DrawDebugPlane(GetWorld(), FPlane(0.f, 0.f, 1.f, TilePos.Z), TilePos, TileSize - TileSizeMinus, LinearColor, 50.f);
}

bool AGrid::GridSpheretrace(FVector& Location, ETraceTypeQuery TraceType, FHitResult& OutHit)
{
	 TArray<AActor* > ActorToIgnore;
	return UKismetSystemLibrary::SphereTraceSingle(this, Location, Location, TileSize - TileSizeMinus, TraceType, true, ActorToIgnore, EDrawDebugTrace::None, OutHit, true);
}

int32 AGrid::GetTileCost(GroundType GroundType)
{
	int32 Cost;
	switch (GroundType)
	{
	case None:
		Cost = 999;
		break;
	case Normal:
		Cost = 1;
		break;
	case Diffictuly:
		Cost = 2;
		break;
	case ReallyDiffictuly:
		Cost = 5;
		break;
	case Impossible:
		Cost = 999;
		break;
	default:
		Cost = 0;
		break;
	}
	return Cost;
}

int32 AGrid::GetEstimatedCostToTarget(FVector2D CurrentTile, FVector2D TargetTile)
{
	FVector2D CalculatedEstimated = CurrentTile - TargetTile;
	float x = FMath::Abs(CalculatedEstimated.X);
	float y = FMath::Abs(CalculatedEstimated.Y);
	return FMath::RoundToInt(x + y);
}

FVector AGrid::GridBottomLeft()
{
	FVector LeftGrid = GridLocation - (GetActorRightVector() * GridSizeWorld.X);
	FVector BottomGrid = GetActorForwardVector() * GridSizeWorld.Y;
	return LeftGrid - BottomGrid;
}

FVector2D AGrid::GridTileNumber()
{
	return GridSizeWorld / TileSize;
}

TArray<FVector2D> AGrid::RetracePath(FVector2D TargetTile, FVector2D StartTile)
{
	FVector2D currenTile = TargetTile;
	TArray<FVector2D> invertedPath;
	TArray<FVector2D> outPath;
	while (!currenTile.Equals(StartTile, 0.00001))
	{
		invertedPath.AddUnique(currenTile);
		currenTile = PathData.Find(currenTile)->PreviousTile;
	}
		
	for (int32 i = invertedPath.Num() - 1; i >= 0; i--)
	{
		outPath.AddUnique(invertedPath[i]);
	}
	return outPath;
}

TArray<FVector2D> AGrid::GetTileNeighbours(FVector2D GridIndex)
{
	TArray<FVector2D> possibleNeighbours = TArray<FVector2D>{ FVector2D(1.f, 0.f), FVector2D(0.f, 1.f),FVector2D(-1.f, 0.f), FVector2D(0.f, -1.f) };
	TArray<FVector2D> outNeighbours;
	for (auto& Possible : possibleNeighbours) {
		
		if (PathData.Find(Possible + GridIndex)) {
			if (PathData.Find(Possible + GridIndex)->GroundTypes != GroundType::None && PathData.Find(Possible + GridIndex)->GroundTypes != GroundType::Impossible && PathData.Find(Possible + GridIndex)->UnitOnThisTile != nullptr) {
				outNeighbours.AddUnique(Possible + GridIndex);
			}
		}
	}
	return outNeighbours;
}