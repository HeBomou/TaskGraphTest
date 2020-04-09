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

    // Fire
    m_bCanFire = true;

    // Move
    m_bCanMove = true;
}

void ATankPawn::BeginPlay() {
    Super::BeginPlay();
}

void ATankPawn::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (m_bCanFire) {
        UWorld* World = GetWorld();
        if (World) {  // TODO: 寻找敌人并决定方向
            m_bCanFire = false;
            World->SpawnActor<ATaskGraphTestProjectile>(GetActorLocation() + FVector(90.0f, 0, 0), GetActorRotation());
            World->GetTimerManager().SetTimer(TimerHandle_LifeSpanExpired, this, &ATankPawn::RecoverWeapon, 0.3f);
        }
    }

    if (m_bCanMove) {
        UWorld* World = GetWorld();
        if (World) {
            m_moveDir = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 0.0f).GetSafeNormal();
            m_bCanMove = false;
            World->GetTimerManager().SetTimer(TimerHandle_LifeSpanExpired, this, &ATankPawn::RecoverMove, 2.0f);
        }
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

void ATankPawn::RecoverWeapon() {
    m_bCanFire = true;
}

void ATankPawn::RecoverMove() {
    m_bCanMove = true;
}

void ATankPawn::BeenHit(int32 dmg) {
    if ((m_health -= dmg) <= 0)
        m_manager->TankDead(m_id);
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
