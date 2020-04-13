#include "TankTG.h"

#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

ATankTG::ATankTG() {
    static ConstructorHelpers::FObjectFinder<UStaticMesh> tankMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));

    PrimaryActorTick.bCanEverTick = false;

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

float ATankTG::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
    m_health -= DamageAmount;
    return DamageAmount;
}

void ATankTG::Move() {
    FHitResult Hit(1.f);
    RootComponent->MoveComponent(m_moveDir, m_moveRot, true, &Hit);

    if (Hit.IsValidBlockingHit()) {
        const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
        const FVector Deflection = FVector::VectorPlaneProject(m_moveDir, Normal2D) * (1.f - Hit.Time);
        RootComponent->MoveComponent(Deflection, m_moveRot, true);
    }
}