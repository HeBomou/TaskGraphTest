#include "TankManagerTG.h"

#include "Engine/World.h"
#include "TankTG.h"

ATankManagerTG::ATankManagerTG() {
    PrimaryActorTick.bCanEverTick = true;

    // Scene Component
    m_scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene0"));
    RootComponent = m_scene;

    // Spawn
    m_spawnRecoverTime = 5;
    m_spawnTimer = 0;
}

void ATankManagerTG::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    UWorld* world = GetWorld();
    if (!world) return;

    // Spawn
    if (m_spawnTimer < 0) {
        m_spawnTimer += m_spawnRecoverTime;
        for (uint32 i = 0; i < 40; i++)
            m_tanks.Add(world->SpawnActor<ATankTG>(FVector(FMath::RandPointInCircle(1500), 0) + GetActorLocation(), FRotator()));
    } else
        m_spawnTimer -= DeltaTime;

    // Tanks
    TArray<ATankTG*> tanks;
    for (auto it = m_tanks.CreateIterator(); it; it++)
        tanks.Add(*it);

    TArray<FVector> shootDirs;
    TArray<FVector> moveDirs;
    shootDirs.Init(FVector(), tanks.Num());
    moveDirs.Init(FVector(), tanks.Num());
    // TODO: Find nearest tank with ParallelFor
    for (int i = 0; i < tanks.Num(); i++) {
        ATankTG* enemy = NULL;
        float mnDis = FLT_MAX;
        for (int j = 0; j < tanks.Num(); j++) {
            auto dis = (tanks[j]->GetActorLocation() - GetActorLocation()).SizeSquared();
            if (tanks[j]->m_teamId != tanks[i]->m_teamId && dis < mnDis)
                enemy = tanks[j], mnDis = dis;
        }
        if (enemy)
            moveDirs[i] = shootDirs[i] = (enemy->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        else {
            shootDirs[i] = FVector();
            while (moveDirs[i].SizeSquared() == 0)
                moveDirs[i] = FVector(FMath::RandPointInCircle(1).GetSafeNormal(), 0);
        }
    }

    for (int i = 0; i < tanks.Num(); i++) {
        auto tank = tanks[i];
        // Move
        auto moveDir = moveDirs[i];
        if (tank->m_moveTimer < 0)
            tank->m_moveTimer += tank->m_moveRecoverTime, tank->m_moveDir = moveDir, tank->m_moveRot = moveDir.Rotation();
        else
            tank->m_moveTimer -= DeltaTime;
        tank->Move();

        // Shoot
		auto shootDir = shootDirs[i];
        if (tank->m_shootTimer < 0) {
            tank->m_shootTimer += tank->m_shootRecoverTime;
			// TODO:
            // world->SpawnActor<ABullet>(GetActorLocation() + shootDir * 80.f, shootDir.Rotation());
        } else
            tank->m_shootTimer -= DeltaTime;
    }

    // Kill Tanks
    TArray<ATankTG*> tanksToRemove;
    for (auto it = m_tanks.CreateIterator(); it; it++) {
        if ((*it)->m_health <= 0)
            tanksToRemove.Add(*it);
    }
    for (auto it = tanksToRemove.CreateIterator(); it; it++) {
        (*it)->Destroy();
        m_tanks.Remove(*it);
    }
}
