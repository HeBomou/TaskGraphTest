#include "TankManagerTG.h"

#include "BulletTG.h"
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

    // Tanks Compute
    TArray<ATankTG*> tanks;
    for (auto it = m_tanks.CreateIterator(); it; ++it)
        tanks.Add(*it);
    TArray<FVector> shootDirs;
    TArray<FVector> moveDirs;
    shootDirs.Init(FVector(), tanks.Num());
    moveDirs.Init(FVector(), tanks.Num());
    // TODO: Find nearest tank with ParallelFor
    for (int32 i = 0; i < tanks.Num(); i++) {
        ATankTG* enemy = NULL;
        float mnDis = FLT_MAX;
        for (int32 j = 0; j < tanks.Num(); j++) {
            if (i == j) continue;
            UE_LOG(LogTemp, Warning, TEXT("Tank Num: %d"), tanks.Num());
            auto dis = (tanks[j]->GetActorLocation() - tanks[i]->GetActorLocation()).SizeSquared();
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
            world->SpawnActor<ABulletTG>(GetActorLocation() + shootDir * 80.f, shootDir.Rotation());
        } else
            tank->m_shootTimer -= DeltaTime;
    }

    // Kill Tanks
    TArray<ATankTG*> tanksToRemove;
    for (auto it = m_tanks.CreateIterator(); it; ++it) {
        if ((*it)->m_health <= 0)
            tanksToRemove.Add(*it);
    }
    for (int i = 0; i < tanksToRemove.Num(); i++) {
        tanksToRemove[i]->Destroy();
        m_tanks.Remove(tanksToRemove[i]);
    }

    // Spawn
    if (m_spawnTimer < 0) {
        m_spawnTimer += m_spawnRecoverTime;
        for (uint32 i = 0; i < 40; i++) {
            auto tank = world->SpawnActor<ATankTG>(FVector(FMath::RandPointInCircle(1500), 0) + GetActorLocation(), FRotator());
            if (tank)
                m_tanks.Add(tank);
        }
    } else
        m_spawnTimer -= DeltaTime;
}
