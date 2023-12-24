// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingCharacter.h"
#include "PathFinding/Controller/PathFindingPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PathFinding/Struct/SPathFindingStruct.h"
#include "PathFinding/Grid/Grid.h"

// Sets default values
APathfindingCharacter::APathfindingCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Cone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cone"));
	Cone->SetupAttachment(GetRootComponent());

	Cone_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cone 1"));
	Cone_1->SetupAttachment(Cone);
}

// Called when the game starts or when spawned
void APathfindingCharacter::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimeHandle;
	GetWorld()->GetTimerManager().SetTimer(TimeHandle, [&]() {
		PlayerController = Cast<APathFindingPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		TArray<AActor*> FindActor;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), GridClass, FindActor);
		Grid = Cast<AGrid>(FindActor[0]);
		SetGridIndexAndSnapToLocation();

		GetCapsuleComponent()->OnClicked.AddUniqueDynamic(this, &APathfindingCharacter::OnClickedBegin);
		Cone->OnClicked.AddUniqueDynamic(this, &APathfindingCharacter::OnClickedBegin);
		Cone_1->OnClicked.AddUniqueDynamic(this, &APathfindingCharacter::OnClickedBegin);
		}, .5f, false);
}

// Called every frame
void APathfindingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveToLocation(DeltaTime);
}

// Called to bind functionality to input
void APathfindingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APathfindingCharacter::OnClickedBegin(UPrimitiveComponent* PrimComp, FKey InKey)
{
	if (PlayerController->SelectedUnit != this) {
		PlayerController->NewUnitSelected(this);
		Selected();
	}
	else {
		PlayerController->NewUnitSelected(nullptr);
	}
}

void APathfindingCharacter::SetGridIndexAndSnapToLocation()
{
	float closestDistance = 9999999.f;
	FVector2d closestIndex;

	for (auto& Data : Grid->PathData) {
		float lengthPathWorldLocation = FVector(Grid->PathData.Find(Data.Key)->WorldLocation - GetActorLocation()).Length();
		if (lengthPathWorldLocation < closestDistance) {
			closestDistance = lengthPathWorldLocation;
			closestIndex = Data.Key;
		}
	}
	GridIndex = closestIndex;
	UnitOnThistile(closestIndex);
	float OffsetZLoc = Grid->PathData.Find(closestIndex)->WorldLocation.Z + GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	SetActorLocation(FVector(Grid->PathData.Find(closestIndex)->WorldLocation.X, Grid->PathData.Find(closestIndex)->WorldLocation.Y, OffsetZLoc));
}

void APathfindingCharacter::MoveToLocation(float DeltaTime)
{
	if (OnLocation) {
		if (LerpValue <= 1) {
			LerpValue += 1 * DeltaTime;
			FVector Endlocation = Grid->GetSplineComponent()->GetLocationAtDistanceAlongSpline(LerpValue * Grid->GetSplineComponent()->GetSplineLength(), ESplineCoordinateSpace::World);
			FRotator EndRotation = Grid->GetSplineComponent()->GetRotationAtDistanceAlongSpline(LerpValue * Grid->GetSplineComponent()->GetSplineLength(), ESplineCoordinateSpace::World);
			SetActorLocationAndRotation(Endlocation, EndRotation);
		}
		else {
			OnLocation = false;
			LerpValue = 0;
			Grid->bIsUnitMoving = false;
		}
	}
}

void APathfindingCharacter::UnitOnThistile(FVector2D _GridIndex)
{
	FPathFindingData NewData;
	NewData.TileActor = Grid->PathData.Find(GridIndex)->TileActor;
	NewData.UnitOnThisTile = nullptr;
	NewData.GroundTypes = Grid->PathData.Find(GridIndex)->GroundTypes;
	NewData.WorldLocation = Grid->PathData.Find(GridIndex)->WorldLocation;
	NewData.GridIndex = Grid->PathData.Find(GridIndex)->GridIndex;
	NewData.TileCost = Grid->PathData.Find(GridIndex)->TileCost;
	NewData.FinalCost = Grid->PathData.Find(GridIndex)->FinalCost;
	NewData.CostFromStart = Grid->PathData.Find(GridIndex)->CostFromStart;
	NewData.EstimatedCostToTarget = Grid->PathData.Find(GridIndex)->EstimatedCostToTarget;
	NewData.PreviousTile = Grid->PathData.Find(GridIndex)->PreviousTile;
	Grid->PathData.Add(GridIndex, NewData);

	GridIndex = _GridIndex;

	FPathFindingData NewData1;
	NewData1.TileActor = Grid->PathData.Find(GridIndex)->TileActor;
	NewData1.UnitOnThisTile = Grid->PathData.Find(GridIndex)->UnitOnThisTile;
	NewData1.GroundTypes = Grid->PathData.Find(GridIndex)->GroundTypes;
	NewData1.WorldLocation = Grid->PathData.Find(GridIndex)->WorldLocation;
	NewData1.GridIndex = Grid->PathData.Find(GridIndex)->GridIndex;
	NewData1.TileCost = Grid->PathData.Find(GridIndex)->TileCost;
	NewData1.FinalCost = Grid->PathData.Find(GridIndex)->FinalCost;
	NewData1.CostFromStart = Grid->PathData.Find(GridIndex)->CostFromStart;
	NewData1.EstimatedCostToTarget = Grid->PathData.Find(GridIndex)->EstimatedCostToTarget;
	NewData1.PreviousTile = Grid->PathData.Find(GridIndex)->PreviousTile;
	Grid->PathData.Add(GridIndex, NewData1);
}

void APathfindingCharacter::MoveAlongThePath(TArray<FVector2D> _Path)
{
	Path = _Path;
	Path.Insert(GridIndex, 0);
	UnitOnThistile(Path.Last());
	Grid->GetSplineComponent()->ClearSplinePoints();
	for (auto& path : Path) {
		FVector Loc = Grid->PathData.Find(path)->WorldLocation;
		float OffsetZLoc = Loc.Z + GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		Grid->GetSplineComponent()->AddSplinePoint(FVector(Loc.X, Loc.Y, OffsetZLoc), ESplineCoordinateSpace::World);
	}
	OnLocation = true;
	LerpValue = 0;
	Grid->bIsUnitMoving = true;
}

void APathfindingCharacter::Selected()
{
	UE_LOG(LogTemp, Warning, TEXT("Selected"));
}

void APathfindingCharacter::Deselected()
{
	UE_LOG(LogTemp, Warning, TEXT("Deselected"));
}

