// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldJF.h"

// Sets default values
AWorldJF::AWorldJF()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWorldJF::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorldJF::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO: Try to add TaskTick interface for Actor and Cmpt
}

