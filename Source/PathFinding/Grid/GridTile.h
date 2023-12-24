// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "GridTile.generated.h"

UCLASS()
class PATHFINDING_API AGridTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetWidgetVariable();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	USceneComponent* SceneRoot;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly)
		UWidgetComponent* WidgetTile;

	class UPathFindingUserWidget* WidgetGridTile;

	bool bIsOvered;

	bool bIsSelected;

	bool bIsInPath;

protected:
	void SetTileColor();

	void SetTileSize();

	struct FPathFindingData* GetTileData();

	UFUNCTION()
		void OnBeginMouseOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
		void OnEndMouseOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
		void OnBeginClickMouse(UPrimitiveComponent* PrimComp, FKey InKey);
public:
	FVector2D GridIndex;

	class AGrid* Grid;

	void SetSelected(bool Selected);

	void SetOvered(bool Overed);

	void IsInPath(bool IsInPath);
};
