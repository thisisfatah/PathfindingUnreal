// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTile.h"
#include "Grid.h"
#include "PathFinding/Widget/PathFindingUserWidget.h"
#include "PathFinding/Struct/SPathFindingStruct.h"

// Sets default values
AGridTile::AGridTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(SceneRoot);

	WidgetTile = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Tile"));
	WidgetTile->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void AGridTile::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimeHandle;
	GetWorld()->GetTimerManager().SetTimer(TimeHandle, [&] {
		SetTileColor();
		SetTileSize();
		SetWidgetVariable();
		},0.3f, false);

	StaticMesh->OnBeginCursorOver.AddUniqueDynamic(this, &AGridTile::OnBeginMouseOver);
	StaticMesh->OnEndCursorOver.AddUniqueDynamic(this, &AGridTile::OnEndMouseOver);
	StaticMesh->OnClicked.AddUniqueDynamic(this, &AGridTile::OnBeginClickMouse);
}

void AGridTile::SetWidgetVariable()
{
	WidgetGridTile = Cast<UPathFindingUserWidget>(WidgetTile->GetUserWidgetObject());
	WidgetGridTile->Grid = Grid;
	WidgetGridTile->GridIndex = GridIndex;
	WidgetGridTile->SetText();
}

// Called every frame
void AGridTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridTile::SetTileColor()
{
	FLinearColor TileColor;
	switch (GetTileData()->GroundTypes)
	{
	case Normal:
		TileColor = FLinearColor::White;
		break;
	case Diffictuly:
		TileColor = FLinearColor::Blue;
		break;
	case ReallyDiffictuly:
		TileColor = FLinearColor::Yellow;
		break;
	case Impossible:
		TileColor = FLinearColor::Red;
		break;
	default:
		TileColor = FLinearColor::Black + (FLinearColor::White * 0.1);
		break;
	}
	
	if (bIsSelected) {
		StaticMesh->SetScalarParameterValueOnMaterials("bIsSelected", 1.f);
	}
	else {
		StaticMesh->SetScalarParameterValueOnMaterials("bIsSelected", 0.f);
	}

	if (bIsInPath) {
		TileColor = FLinearColor::Blue + FLinearColor::White;
	}

	if (bIsOvered) {
		TileColor = TileColor * 3;
	}
	StaticMesh->SetVectorParameterValueOnMaterials("TileColor", FVector(TileColor));
}

void AGridTile::SetTileSize()
{
	float TileSize = Grid->TileSize / 50;
	FVector NewScale = StaticMesh->GetComponentScale() * TileSize;
	StaticMesh->SetWorldScale3D(FVector(NewScale.X, NewScale.Y, StaticMesh->GetComponentScale().Z));
}

FPathFindingData* AGridTile::GetTileData()
{
	return Grid->PathData.Find(GridIndex);
}

void AGridTile::OnBeginMouseOver(UPrimitiveComponent* TouchedComponent)
{
	SetOvered(true);
	Grid->SetOveredTile(this);
}

void AGridTile::OnEndMouseOver(UPrimitiveComponent* TouchedComponent)
{
	Grid->SetOveredTile(nullptr);
}

void AGridTile::OnBeginClickMouse(UPrimitiveComponent* PrimComp, FKey InKey)
{
	SetSelected(true);
	Grid->SetSelectedTile(this);
}

void AGridTile::SetSelected(bool Selected)
{
	bIsSelected = Selected;
	SetTileColor();
}

void AGridTile::SetOvered(bool Overed)
{
	bIsOvered = Overed;
	SetTileColor();
}

void AGridTile::IsInPath(bool IsInPath)
{
	bIsInPath = IsInPath;
	SetTileColor();
	SetWidgetVariable();
}

