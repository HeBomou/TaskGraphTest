#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TankSpawner.generated.h"

UCLASS()
class TGTEST_API ATankSpawner : public AActor {
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Scene, meta = (AllowPrivateAccess = "true"))
    USceneComponent *m_scene;

   public:
    ATankSpawner();

   public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

   private:
    uint32 m_spawnNum;
    float m_spawnRecoverTime;
    float m_spawnTimer;
};
