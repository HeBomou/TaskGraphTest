// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TestSelf.generated.h"

UCLASS()
class TGTEST_API ATestSelf : public APawn {
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* m_selfHullMesh;

    UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* m_cameraComponent;

    UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* m_cameraBoom;

   protected:
    float m_health;
    float m_shootRecoverTime;
    float m_shootTimer;

   public:
    ATestSelf();

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    // Static names for axis bindings
    static const FName s_moveForwardBinding;
    static const FName s_moveRightBinding;
    static const FName s_fireForwardBinding;
    static const FName s_fireRightBinding;
};
