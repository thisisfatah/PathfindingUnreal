// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PathFindingUserWidget.generated.h"

class UTextBlock;
class AGrid;

/**
 * 
 */
UCLASS()
class PATHFINDING_API UPathFindingUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

protected:
	/// <summary>
	/// Used to bind text interact
	/// </summary>
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Text_FinalCost;

	/// <summary>
	/// Used to bind text interact
	/// </summary>
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Text_TileCost;

	FText GetFinalCost();

	FText GetTileCost();

public:
	FVector2D GridIndex;

	AGrid* Grid;

public:
	void SetText();
};
