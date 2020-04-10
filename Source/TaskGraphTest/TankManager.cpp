#include "TankManager.h"

#include "Components/SceneComponent.h"
#include "TankPawn.h"
#include "TimerManager.h"

ATankManager::ATankManager() {
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TankManager"));

    m_bCanSpawn = true;
    
    m_idCounter = 0;
}

void ATankManager::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (m_bCanSpawn) {
        // 生成多个ATankPawn
        UWorld* World = GetWorld();
        if (World) {
            for (int i = 0; i < 5; i++) {
                float x = FMath::RandRange(-500.f, 500.f);
                float y = FMath::RandRange(-500.f, 500.f);
                ATankPawn* tank = World->SpawnActor<ATankPawn>(GetActorLocation() + FVector(x, y, 0), GetActorRotation());
                while (m_tankMap.Contains(++m_idCounter))
                    ;  // 避免id重复
                m_tankMap.Add(m_idCounter, tank);
                // tank->Init(m_idCounter, FMath::RandRange(1, 3), this);
            }
            m_bCanSpawn = false;
            World->GetTimerManager().SetTimer(TimerHandle_LifeSpanExpired, this, &ATankManager::RecoverSpawn, 10);
        }
    }
}

void ATankManager::RecoverSpawn() {
    m_bCanSpawn = true;
}

void ATankManager::TankDead(int32 id) {
    m_tankMap.Remove(id);
}
