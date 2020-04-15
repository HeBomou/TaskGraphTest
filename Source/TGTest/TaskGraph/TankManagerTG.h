#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TankManagerTG.generated.h"

class ATankTG;

UCLASS()
class TGTEST_API ATankManagerTG : public AActor {
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Scene, meta = (AllowPrivateAccess = "true"))
    USceneComponent *m_scene;

   public:
    ATankManagerTG();

   public:
    virtual void Tick(float DeltaTime) override;

   private:
    TSet<ATankTG *> m_tanks;
    float m_spawnRecoverTime;
    float m_spawnTimer;
};
