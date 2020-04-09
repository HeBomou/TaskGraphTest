#include "TankPawn.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "TaskGraphTestProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ATankPawn::ATankPawn() {
    // Mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TankMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
    TankMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankMesh"));
    RootComponent = TankMeshComponent;
    TankMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    TankMeshComponent->SetStaticMesh(TankMesh.Object);

    // Fire
    bCanFire = true;
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay() {
    Super::BeginPlay();
}

// Called every frame
void ATankPawn::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (bCanFire) {
        UWorld* World = GetWorld();
        if (World)
            World->SpawnActor<ATaskGraphTestProjectile>(GetActorLocation() + FVector(90.0f, 0, 0), GetActorRotation());
        bCanFire = false;
        World->GetTimerManager().SetTimer(TimerHandle_LifeSpanExpired, this, &ATankPawn::RecoverWeapon, 0.3f);
    }
}

void ATankPawn::RecoverWeapon() {
    bCanFire = true;
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
