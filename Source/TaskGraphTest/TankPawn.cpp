#include "TankPawn.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "TankManager.h"
#include "Bullet.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

ATankPawn::ATankPawn() {
    // Mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TankMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
    m_tankMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankMesh"));
    RootComponent = m_tankMeshComponent;
    m_tankMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    m_tankMeshComponent->SetStaticMesh(TankMesh.Object);
}

void ATankPawn::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    m_fireTimer -= DeltaTime;
    if (m_fireTimer <= 0) {
        m_fireTimer += 0.5f;
        UWorld* World = GetWorld();
        if (World) {  // TODO: 寻找敌人并决定方向
            World->SpawnActor<ABullet>(GetActorLocation() + GetActorRotation().Vector() * 90.f, GetActorRotation());
        }
    }

    m_moveTimer -= DeltaTime;
    if (m_moveTimer <= 0) {
        m_moveTimer += 3.f;
        m_moveDir = FVector(FMath::RandRange(-1.f, 1.f), FMath::RandRange(-1.f, 1.f), 0.f).GetSafeNormal();
    }

    if (m_moveDir.SizeSquared2D() > 0.f) {
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

