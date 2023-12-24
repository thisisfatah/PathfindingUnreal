// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PathFindingPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PATHFINDING_API APathFindingPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	class APathfindingCharacter* SelectedUnit;

public:
	void NewUnitSelected(class APathfindingCharacter* Unit);

	void NewPathAvaible(TArray<FVector2D> Path);
};
