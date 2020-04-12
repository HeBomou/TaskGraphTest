#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class UWorld;
class UStaticMeshComponent;

UCLASS()
class TGTEST_API ATank : public APawn {
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent *m_tankMesh;

   public:
    ATank();

   public:
    virtual void Tick(float DeltaTime) override;

   private:
    uint32 m_teamId;
    float m_moveRecoverTime;
    float m_moveTimer;
	FVector m_moveDir;
	FRotator m_moveRot;
    float m_shootRecoverTime;
    float m_shootTimer;
    inline void TryMove(FVector moveDir, float dT);
    inline void TryShoot(FVector shootDir, float dT, UWorld *world);
};
