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
	class UStaticMeshComponent* m_tankMeshComponent;

public:
	ATankPawn();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	// void Init(int32 id, int32 teamId, ATankManager* manager);

private:

	uint32 m_id;

	uint32 m_teamId;

	ATankManager* m_manager;

	float m_health;

	float m_fireTimer;

	float m_moveTimer;

	FVector m_moveDir;

};
