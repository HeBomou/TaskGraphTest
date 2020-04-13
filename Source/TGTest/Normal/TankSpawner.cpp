#include "TankSpawner.h"

#include "Engine/World.h"
#include "Tank.h"

ATankSpawner::ATankSpawner() {
    PrimaryActorTick.bCanEverTick = true;

	// Scene Component
	m_scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene0"));
	RootComponent = m_scene;

    m_spawnNum = 40;
    m_spawnRecoverTime = 5;
    m_spawnTimer = 0;
}

void ATankSpawner::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    UWorld *world = GetWorld();
    if (!world) return;

    if (m_spawnTimer < 0) {
        m_spawnTimer += m_spawnRecoverTime;
        for (uint32 i = 0; i < m_spawnNum; i++)
            world->SpawnActor<ATank>(FVector(FMath::RandPointInCircle(1500), 0) + GetActorLocation(), FRotator());
    } else
        m_spawnTimer -= DeltaTime;
}
