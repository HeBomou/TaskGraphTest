// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldJF.generated.h"

class ATestEnemy;

UCLASS()
class TGTEST_API AWorldJF : public AActor {
    GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Scene, meta = (AllowPrivateAccess = "true"))
	USceneComponent *m_scene;

   public:
    // Sets default values for this actor's properties
    AWorldJF();

   protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

   public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

	private:
	TSet<ATestEnemy*> m_tanks;
	float m_spawnRecoverTime;
	float m_spawnTimer;
};
