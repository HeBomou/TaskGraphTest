#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TankManager.generated.h"

UCLASS()
class TASKGRAPHTEST_API ATankManager : public AActor {
    GENERATED_BODY()

   public:
    ATankManager();
	static ATankManager* GetInstance();

   protected:
    virtual void BeginPlay() override;

   public:
    virtual void Tick(float DeltaTime) override;

    void RecoverSpawn();

	void TankDead(int32 id);

   private:
    bool m_bCanSpawn;

	TMap<int32, class ATankPawn*> m_tankMap;


};
