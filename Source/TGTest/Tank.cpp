#include "Tank.h"

#include "Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

ATank::ATank() {
    static ConstructorHelpers::FObjectFinder<UStaticMesh> tankMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));

    // Mesh Component
    m_tankMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankMesh0"));
    RootComponent = m_tankMesh;
    m_tankMesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    m_tankMesh->SetStaticMesh(tankMesh.Object);

    // Team
    m_teamId = FMath::RandRange(1, 3);

    // Health
    m_health = 100;

    // Move Recover
    m_moveRecoverTime = 3.f;
    m_moveTimer = FMath::RandRange(0.f, 3.f);

    // Shoot Recover
    m_shootRecoverTime = 0.3f;
    m_shootTimer = 0.3f;
}

void ATank::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (m_health <= 0)
        Destroy();

    UWorld *world = GetWorld();
    if (!world)
        return;

    TArray<AActor *> tanks;
    UGameplayStatics::GetAllActorsOfClass(world, ATank::StaticClass(), tanks);

    // Find the nearest enemy
    AActor *enemy = NULL;
    float mnDis = FLT_MAX;
    for (AActor *tank : tanks) {
        auto dis = (tank->GetActorLocation() - GetActorLocation()).SizeSquared();
        if (((ATank *)tank)->m_teamId != m_teamId && dis < mnDis)
            enemy = tank, mnDis = dis;
    }
    FVector moveDir(0), shootDir;
    if (enemy)
        moveDir = shootDir = (enemy->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    else {
        shootDir = GetActorForwardVector();
        while (moveDir.SizeSquared() == 0)
            moveDir = FVector(FMath::RandPointInCircle(1).GetSafeNormal(), 0);
    }

    TryMove(moveDir, DeltaTime);
    TryShoot(shootDir, DeltaTime, world);
}

float ATank::TakeDamage(float DamageAmount, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser) {
    m_health -= DamageAmount;
    return DamageAmount;
}

void ATank::TryMove(FVector moveDir, float dT) {
    if (m_moveTimer < 0)
        m_moveTimer += m_moveRecoverTime, m_moveDir = moveDir, m_moveRot = moveDir.Rotation();
    else
        m_moveTimer -= dT;

    FHitResult Hit(1.f);
    RootComponent->MoveComponent(m_moveDir, m_moveRot, true, &Hit);

    if (Hit.IsValidBlockingHit()) {
        const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
        const FVector Deflection = FVector::VectorPlaneProject(m_moveDir, Normal2D) * (1.f - Hit.Time);
        RootComponent->MoveComponent(Deflection, m_moveRot, true);
    }
}

void ATank::TryShoot(FVector shootDir, float dT, UWorld *world) {
    if (m_shootTimer < 0) {
        m_shootTimer += m_shootRecoverTime;
        world->SpawnActor<ABullet>(GetActorLocation() + shootDir * 80.f, shootDir.Rotation());
    } else
        m_shootTimer -= dT;
}
