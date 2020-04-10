#include "TankPawn.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "TankManager.h"
#include "TaskGraphTestProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

ATankPawn::ATankPawn() {
    // Mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TankMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
    TankMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankMesh"));
    RootComponent = TankMeshComponent;
    TankMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    TankMeshComponent->SetStaticMesh(TankMesh.Object);
}

void ATankPawn::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    m_fireTimer -= DeltaTime;
    if (m_fireTimer <= 0) {
        m_fireTimer += 0.5f;
        UWorld* World = GetWorld();
        if (World) {  // TODO: 寻找敌人并决定方向
            World->SpawnActor<ATaskGraphTestProjectile>(GetActorLocation() + GetActorRotation().Vector() * 90.0f, GetActorRotation());
        }
    }

    m_moveTimer -= DeltaTime;
    if (m_moveTimer <= 0) {
        m_moveTimer += 3.0f;
        m_moveDir = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 0.0f).GetSafeNormal();
    }

    if (m_moveDir.SizeSquared2D() > 0.0f) {
        const FRotator NewRotation = m_moveDir.Rotation();
        FHitResult Hit(1.f);
        RootComponent->MoveComponent(m_moveDir, NewRotation, true, &Hit);

        if (Hit.IsValidBlockingHit()) {
            const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
            const FVector Deflection = FVector::VectorPlaneProject(m_moveDir, Normal2D) * (1.f - Hit.Time);
            RootComponent->MoveComponent(Deflection, NewRotation, true);
        }
    }
}

void ATankPawn::Init(int32 id, int32 teamId, ATankManager* manager) {
    m_id = id;
    m_teamId = teamId;
    m_manager = manager;
}

void ATankPawn::BeenHit(int32 dmg) {
    m_health -= dmg;
    m_manager->TankDead(m_id);
}

