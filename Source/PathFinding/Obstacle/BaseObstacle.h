// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "BaseObstacle.generated.h"

UENUM()
enum GroundType
{
	None,
	Normal,
	Diffictuly,
	ReallyDiffictuly,
	Impossible
};

UCLASS()
class PATHFINDING_API ABaseObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	USceneComponent* SceneRoot;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* StaticMesh;

public:
	UPROPERTY(EditAnywhere, Category = "Base Obstacle")
		TEnumAsByte<GroundType> GroundTypes;
};
