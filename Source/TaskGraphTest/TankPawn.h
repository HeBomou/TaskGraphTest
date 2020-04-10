// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

UCLASS()
class TASKGRAPHTEST_API ATankPawn : public APawn
{
	GENERATED_BODY()

	// Mesh
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TankMeshComponent;

public:
	// Sets default values for this pawn's properties
	ATankPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(int32 id, int32 teamId);
	
	void RecoverWeapon();

	void RecoverMove();

	void BeenHit(int32 dmg);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	uint32 m_id;

	uint32 m_teamId;

	// class ATankManager* m_manager;

	int32 m_health;

	float m_fireTimer;

	float m_moveTimer;

	FVector m_moveDir;

};