// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BillboardComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SplineComponent.h"
#include "PathFinding/Obstacle/BaseObstacle.h"
#include "PathFinding/Struct/SPathFindingStruct.h"
#include "Grid.generated.h"

UCLASS()
class PATHFINDING_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// Called when an instance of this class is placed (in editor) or spawned.
	virtual void OnConstruction(const FTransform& Transform) override;

	void DrawTile();

	void GenerateMapDataFromWorld();

	void GridDrawPlane(FVector& TilePos, TEnumAsByte<GroundType> TypeGround);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	USceneComponent* SceneRoot;

	UPROPERTY(EditDefaultsOnly)
		UBillboardComponent* Billboard;

	UPROPERTY(EditDefaultsOnly)
		USplineComponent* Spline;

	UPROPERTY(EditAnywhere, Category = "Grid")
		FVector GridLocation;

	UPROPERTY(EditAnywhere, Category = "Grid")
		FVector2D GridSizeWorld;

	UPROPERTY(EditAnywhere, Category = "Grid")
		float TileSizeMinus;

	UPROPERTY(EditAnywhere, Category = "Grid")
		TSubclassOf<class AGridTile> GridTile;

	class AGridTile* SelectedTile;

	class AGridTile* OveredTile;

	class APathFindingPlayerController* PlayerController;

	TArray<FVector2D> Path;

	bool debugJobCompleted;

public:
	UPROPERTY(EditAnywhere, Category = "Grid")
		TMap<FVector2D, FPathFindingData> PathData;

	UPROPERTY(EditAnywhere, Category = "Grid")
		float TileSize = 60;

	bool bIsUnitMoving;

	void SetSelectedTile(class AGridTile* NewSelectedtile);

	void SetOveredTile(class AGridTile* NewOveredTile);

	void HighlightCurrenPath(bool bIsForHighlight);

	int32 GetTileCost(GroundType GroundType);

	FORCEINLINE USplineComponent* GetSplineComponent() { return Spline; };

protected:
	void SpawnTile(bool SpawnNoneTile);

	void FindPathToTarget(FVector2D StartTile, FVector2D TargetTile);

	void AddNewPathData(FVector2D& currentNeighbour, const FVector2D& targetTile, const int32& CurrentNeighbourCostFromStart, const FVector2D& currentTile);

	bool GridSpheretrace(FVector& Location, ETraceTypeQuery TraceType, FHitResult& OutHit);

	FVector GridBottomLeft();

	FVector2D GridTileNumber();

	TArray<FVector2D> RetracePath(FVector2D startTile, FVector2D targetTile);

	TArray<FVector2D> GetTileNeighbours(FVector2D GridIndex);

	int32 GetEstimatedCostToTarget(FVector2D CurrentTile, FVector2D TargetTile);
};
