// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFindingPlayerController.h"
#include "PathFinding/Character/PathfindingCharacter.h"

void APathFindingPlayerController::NewUnitSelected(APathfindingCharacter* Unit)
{
	if (IsValid(SelectedUnit)) {
		SelectedUnit->Deselected();
		SelectedUnit = Unit;
	}
	else {
		SelectedUnit = Unit;
	}
}

void APathFindingPlayerController::NewPathAvaible(TArray<FVector2D> Path)
{
	if (IsValid(SelectedUnit))
		SelectedUnit->MoveAlongThePath(Path);
}
