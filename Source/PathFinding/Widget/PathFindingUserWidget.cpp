// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFindingUserWidget.h"
#include "Components/TextBlock.h"
#include "PathFinding/Grid/Grid.h"

void UPathFindingUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void UPathFindingUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

FText UPathFindingUserWidget::GetFinalCost()
{
	if (IsValid(Grid)) {
		int32 FinalCost = Grid->PathData.Find(GridIndex)->FinalCost;
		int32 StartCost = Grid->PathData.Find(GridIndex)->CostFromStart;
		if (FinalCost != 999) {
			return FText::FromString(FString::FromInt(StartCost));
		}
	}
	return FText::FromString("");
}

FText UPathFindingUserWidget::GetTileCost()
{
	if (IsValid(Grid)) {
		int32 TileCost = Grid->PathData.Find(GridIndex)->TileCost;
		return FText::FromString(FString::FromInt(TileCost));
	}

	return FText::FromString("");
}

void UPathFindingUserWidget::SetText()
{
	Text_FinalCost->SetText(GetFinalCost());
	Text_TileCost->SetText(GetTileCost());
}
