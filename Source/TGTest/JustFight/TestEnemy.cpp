#include "TestEnemy.h"

#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

ATestEnemy::ATestEnemy() {
    static ConstructorHelpers::FObjectFinder<UStaticMesh> hullMesh(TEXT("/Game/JustFight/Berserker_BerserkerHull.Berserker_BerserkerHull"));

    // Disable tick
    PrimaryActorTick.bCanEverTick = false;

    // Root cmpt to rotate the mesh
    m_scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneJF"));
    m_scene->SetWorldRotation(FRotator(0.f, 90.f, 0.f));
    RootComponent = m_scene;

    // Mesh
    m_hullMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HullJF"));
    m_hullMesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    m_hullMesh->SetStaticMesh(hullMesh.Object);
    m_hullMesh->AttachToComponent(m_scene, FAttachmentTransformRules::KeepRelativeTransform);

    // Health
    m_health = 100;

    // Move
    m_moveRecoverTime = 3.5f;
    m_moveTimer = 0;

    // Shoot
    m_shoot = false;
    m_shootRecoverTime = 0.8f;
    m_shootTimer = FMath::RandRange(0.f, 0.2f);
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
    FRotator rot = m_moveDir.Rotation();
    m_hullMesh->MoveComponent(m_moveDir * 2.f, rot, true, &Hit);

    if (Hit.IsValidBlockingHit()) {
        const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
        const FVector Deflection = FVector::VectorPlaneProject(m_moveDir, Normal2D) * (1.f - Hit.Time);
        m_hullMesh->MoveComponent(Deflection, rot, true);
    }

    // Shoot
    if (m_shoot) {
        UWorld* world = GetWorld();
        // if (world)
        //     world->SpawnActor<ATestBullet>(GetActorLocation() + m_shootDir * 80.f, m_shootDir.Rotation());
    }
}

void ATestEnemy::MyTaskTick(float dT) {
    // Rand Move
    if (m_moveTimer < 0) {
        m_moveTimer = m_moveRecoverTime;
        while (m_moveDir.SizeSquared() == 0)
            m_moveDir = FVector(FMath::RandPointInCircle(1.f).GetSafeNormal(), 0);
		UE_LOG(LogTemp, Warning, TEXT("%f %f %f"), m_moveDir.X, m_moveDir.Y, m_moveDir.Z);
    } else
        m_moveTimer -= dT;

    // Rand Shoot
    if (m_shootTimer < 0) {
        m_shootTimer = m_shootRecoverTime;
        while (m_shootDir.SizeSquared() == 0)
            m_shootDir = FVector(FMath::RandPointInCircle(1.f).GetSafeNormal(), 0);
        m_shoot = true;
    } else
        m_shootTimer -= dT;
}
