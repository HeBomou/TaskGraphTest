#include "TestSelf.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "TestBullet.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

const FName ATestSelf::s_moveForwardBinding("MoveForward");
const FName ATestSelf::s_moveRightBinding("MoveRight");
const FName ATestSelf::s_fireForwardBinding("FireForward");
const FName ATestSelf::s_fireRightBinding("FireRight");

ATestSelf::ATestSelf() {
    static ConstructorHelpers::FObjectFinder<UStaticMesh> hullMesh(TEXT("/Game/JustFight/Cheap_CheapHull.Cheap_CheapHull"));

    // Disable tick
    PrimaryActorTick.bCanEverTick = true;

    // Mesh
    m_selfHullMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelfHullJF"));
    m_selfHullMesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    m_selfHullMesh->SetStaticMesh(hullMesh.Object);
    RootComponent = m_selfHullMesh;

    // Camera boom
    m_cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    m_cameraBoom->SetupAttachment(RootComponent);
    m_cameraBoom->SetUsingAbsoluteRotation(true);  // Don't want arm to rotate when ship does
    m_cameraBoom->TargetArmLength = 1200.f;
    m_cameraBoom->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
    m_cameraBoom->bDoCollisionTest = false;  // Don't want to pull camera in when it collides with level

    // Camera
    m_cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    m_cameraComponent->SetupAttachment(m_cameraBoom, USpringArmComponent::SocketName);
    m_cameraComponent->bUsePawnControlRotation = false;  // Camera does not rotate relative to arm

    // Health
    m_health = 1000;

    // Shoot
    m_shootRecoverTime = 0.8f;
    m_shootTimer = FMath::RandRange(0.f, m_shootRecoverTime);
}

void ATestSelf::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
    check(PlayerInputComponent);

    // set up gameplay key bindings
    PlayerInputComponent->BindAxis(s_moveForwardBinding);
    PlayerInputComponent->BindAxis(s_moveRightBinding);
    PlayerInputComponent->BindAxis(s_fireForwardBinding);
    PlayerInputComponent->BindAxis(s_fireRightBinding);
}

void ATestSelf::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    // Input
    const float forwardValue = GetInputAxisValue(s_moveForwardBinding);
    const float rightValue = GetInputAxisValue(s_moveRightBinding);
    const float fireForwardValue = GetInputAxisValue(s_fireForwardBinding);
    const float fireRightValue = GetInputAxisValue(s_fireRightBinding);

    // Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
    const FVector moveDir = FVector(forwardValue, rightValue, 0.f).GetSafeNormal();
    const FVector fireDir = FVector(fireForwardValue, fireRightValue, 0.f).GetSafeNormal();

    // Move
    if (moveDir.SizeSquared() > 0) {
        FHitResult Hit(1.f);
        FRotator rot = moveDir.Rotation() + FRotator(0.f, 90.f, 0.f);
        RootComponent->MoveComponent(moveDir * 30.f, rot, true, &Hit);

        if (Hit.IsValidBlockingHit()) {
            const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
            const FVector Deflection = FVector::VectorPlaneProject(moveDir, Normal2D) * (1.f - Hit.Time);
            RootComponent->MoveComponent(Deflection, rot, true);
        }
    }

    // Shoot
    if (m_shootTimer < 0) {
        if (fireDir.SizeSquared() > 0) {
            m_shootTimer = m_shootRecoverTime;
            UWorld* world = GetWorld();
            if (world)
                world->SpawnActor<ATestBullet>(GetActorLocation() + fireDir * 180.f, fireDir.Rotation());
        }
    } else
        m_shootTimer -= DeltaTime;

    // Dead
    if (m_health <= 0)
        Destroy();
}

float ATestSelf::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
    m_health -= DamageAmount;
    return DamageAmount;
}
