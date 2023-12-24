// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "PathfindingCharacter.generated.h"

UCLASS()
class PATHFINDING_API APathfindingCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APathfindingCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* Cone;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* Cone_1;

	UPROPERTY(EditAnywhere, Category = "PatFindingCharacter")
		class APathFindingPlayerController* PlayerController;


	class AGrid* Grid;

	UPROPERTY(EditAnywhere, Category = "PatFindingCharacter")
		TSubclassOf<class AGrid> GridClass;

	TArray<FVector2D> Path;

	FVector StartLoc;
	FVector EndLoc;

	FRotator StartRot;
	FRotator EndRot;

	float LerpValue;

	bool OnLocation;
public:
	FVector2D GridIndex;

protected:
	UFUNCTION()
		void OnClickedBegin(UPrimitiveComponent* PrimComp, FKey InKey);

	void SetGridIndexAndSnapToLocation();

	void MoveToLocation(float DeltaTime);

	void UnitOnThistile(FVector2D _GridIndex);

public:
	void Selected();
	void Deselected();
	void MoveAlongThePath(TArray<FVector2D> _Path);
};
