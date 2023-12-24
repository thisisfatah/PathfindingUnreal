// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PathFinding/Obstacle/BaseObstacle.h"
#include "PathFinding/Grid/GridTile.h"
#include "PathFinding/Character/PathfindingCharacter.h"
#include "SPathFindingStruct.generated.h"


USTRUCT()
struct FPathFindingData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		AGridTile* TileActor;
	UPROPERTY(EditAnywhere)
		APathfindingCharacter* UnitOnThisTile;
	UPROPERTY(EditAnywhere)
		TEnumAsByte<GroundType> GroundTypes;
	UPROPERTY(EditAnywhere)
		FVector WorldLocation;
	UPROPERTY(EditAnywhere)
		FVector2D GridIndex;
	UPROPERTY(EditAnywhere)
		int32 TileCost= 0;
	UPROPERTY(EditAnywhere)
		int32 FinalCost = 0;
	UPROPERTY(EditAnywhere)
		int32 CostFromStart = 0;
	UPROPERTY(EditAnywhere)
		int32 EstimatedCostToTarget = 0;
	UPROPERTY(EditAnywhere)
		FVector2D PreviousTile;
};
