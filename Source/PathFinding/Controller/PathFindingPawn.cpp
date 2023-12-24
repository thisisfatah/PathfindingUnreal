// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFindingPawn.h"

// Sets default values
APathFindingPawn::APathFindingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APathFindingPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathFindingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APathFindingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

