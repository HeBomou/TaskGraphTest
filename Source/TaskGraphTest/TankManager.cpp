#include "TankManager.h"

#include "Components/SceneComponent.h"
#include "TankPawn.h"
#include "TimerManager.h"

ATankManager::ATankManager() {
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TankManager"));

    bCanSpawn = true;
}

void ATankManager::BeginPlay() {
    Super::BeginPlay();
}

void ATankManager::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (bCanSpawn) {
        // 生成多个ATankPawn
        UWorld* World = GetWorld();
        if (World) {
            for (int i = 0; i < 10; i++) {
                float x = FMath::RandRange(-500.0f, 500.0f);
                float y = FMath::RandRange(-500.0f, 500.0f);
                World->SpawnActor<ATankPawn>(GetActorLocation() + FVector(x, y, 0), GetActorRotation());
            }
        }
        bCanSpawn = false;
        World->GetTimerManager().SetTimer(TimerHandle_LifeSpanExpired, this, &ATankManager::RecoverSpawn, 10);
    }
}

void ATankManager::RecoverSpawn() {
    bCanSpawn = true;
}