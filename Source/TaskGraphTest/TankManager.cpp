#include "TankManager.h"

#include "Components/SceneComponent.h"
#include "TankPawn.h"
#include "TimerManager.h"

ATankManager::ATankManager() {
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TankManager"));

    m_bCanSpawn = true;
}

void ATankManager::BeginPlay() {
    Super::BeginPlay();
}

void ATankManager::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    // Id 计数器 爆int自动循环
    static int32 idCounter = 0;

    if (m_bCanSpawn) {
        // 生成多个ATankPawn
        UWorld* World = GetWorld();
        if (World) {
            for (int i = 0; i < 10; i++) {
                float x = FMath::RandRange(-500.0f, 500.0f);
                float y = FMath::RandRange(-500.0f, 500.0f);
                ATankPawn* tank = World->SpawnActor<ATankPawn>(GetActorLocation() + FVector(x, y, 0), GetActorRotation());
                while (m_tankMap.Contains(++idCounter))
                    ;  // 避免id重复
                m_tankMap.Add(idCounter, tank);
                // TODO: 问题在这里
                tank->Init(idCounter, FMath::RandRange(1, 3), this);
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