// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

class ATankManager;

UCLASS()
class TASKGRAPHTEST_API ATankPawn : public APawn
{
	GENERATED_BODY()

	// Mesh
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TankMeshComponent;

public:
	ATankPawn();

public:	
	virtual void Tick(float DeltaTime) override;

	void Init(int32 id, int32 teamId, ATankManager* manager);
	
	void RecoverWeapon();

	void RecoverMove();

	void BeenHit(int32 dmg);

private:

	uint32 m_id;

	uint32 m_teamId;

	ATankManager* m_manager;

	int32 m_health;

	float m_fireTimer;

	float m_moveTimer;

	FVector m_moveDir;

};
