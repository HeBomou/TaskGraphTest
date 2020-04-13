// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankTG.generated.h"

UCLASS()
class TGTEST_API ATankTG : public APawn {
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* m_tankMesh;

   public:
    ATankTG();
    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

   public:
    uint32 m_teamId;
    float m_health;
    float m_moveRecoverTime;
    float m_moveTimer;
    FVector m_moveDir;
    FRotator m_moveRot;
    float m_shootRecoverTime;
    float m_shootTimer;

    void Move();
};
