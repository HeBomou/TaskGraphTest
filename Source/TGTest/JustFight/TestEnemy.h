// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TestEnemy.generated.h"

UCLASS()
class TGTEST_API ATestEnemy : public APawn {
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* m_hullMesh;

   public:
    ATestEnemy();

   protected:
    float m_shootRecoverTime;
    float m_shootTimer;
    bool m_shoot;
    FVector m_shootDir;
    float m_moveRecoverTime;
    float m_moveTimer;
    FVector m_moveDir;

    virtual void BeginPlay() override;

   public:
    float m_health;

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    void MyTick(float dT);
    void MyTaskTick(float dT);
};
