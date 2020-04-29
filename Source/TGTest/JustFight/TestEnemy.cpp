#include "TestEnemy.h"

#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "TestBullet.h"
#include "UObject/ConstructorHelpers.h"

ATestEnemy::ATestEnemy() {
    static ConstructorHelpers::FObjectFinder<UStaticMesh> hullMesh(TEXT("/Game/JustFight/Berserker_BerserkerHull.Berserker_BerserkerHull"));

    // Disable tick
    PrimaryActorTick.bCanEverTick = false;

    // Mesh
    m_hullMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HullJF"));
    m_hullMesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    m_hullMesh->SetStaticMesh(hullMesh.Object);
    RootComponent = m_hullMesh;

    // Health
    m_health = 100;

    // Move
    m_moveRecoverTime = 3.5f;
    m_moveTimer = FMath::RandRange(0.f, m_moveRecoverTime);

    // Shoot
    m_shoot = false;
    m_shootRecoverTime = 2.8f;
    m_shootTimer = FMath::RandRange(0.f, m_shootRecoverTime);
}

void ATestEnemy::BeginPlay() {
    Super::BeginPlay();
}

float ATestEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
    m_health -= DamageAmount;
    return DamageAmount;
}

void ATestEnemy::MyTick(float dT) {
    // Move
    FHitResult Hit(1.f);
    FRotator rot = m_moveDir.Rotation() + FRotator(0.f, 90.f, 0.f);
    m_hullMesh->MoveComponent(m_moveDir * 2.f, rot, true, &Hit);

    if (Hit.IsValidBlockingHit()) {
        const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
        const FVector Deflection = FVector::VectorPlaneProject(m_moveDir, Normal2D) * (1.f - Hit.Time);
        m_hullMesh->MoveComponent(Deflection, rot, true);
    }

    // Shoot
    if (m_shoot) {
		m_shoot = false;
        UWorld* world = GetWorld();
        if (world)
            world->SpawnActor<ATestBullet>(GetActorLocation() + m_shootDir * 220.f, m_shootDir.Rotation());
    }

	// Dead
	if (m_health <= 0)
		Destroy();
}

void ATestEnemy::MyTaskTick(float dT) {
    // Rand Move
    if (m_moveTimer < 0) {
        m_moveTimer = m_moveRecoverTime;
        do
            m_moveDir = FVector(FMath::RandPointInCircle(1.f).GetSafeNormal(), 0);
        while (m_moveDir.SizeSquared() == 0);
    } else
        m_moveTimer -= dT;

    // Rand Shoot
    if (m_shootTimer < 0) {
        m_shootTimer = m_shootRecoverTime;
        do
            m_shootDir = FVector(FMath::RandPointInCircle(1.f).GetSafeNormal(), 0);
        while (m_shootDir.SizeSquared() == 0);
        m_shoot = true;
    } else
        m_shootTimer -= dT;
}
