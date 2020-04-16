// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TGTestPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "TGTestProjectile.h"
#include "TaskAnt/AntEvent.h"
#include "TaskAnt/AntManager.h"
#include "TaskAnt/AntTask.h"
#include "TaskAnt/AntWatcher.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

const FName ATGTestPawn::MoveForwardBinding("MoveForward");
const FName ATGTestPawn::MoveRightBinding("MoveRight");
const FName ATGTestPawn::FireForwardBinding("FireForward");
const FName ATGTestPawn::FireRightBinding("FireRight");

ATGTestPawn::ATGTestPawn() {
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
    // Create the mesh component
    ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
    RootComponent = ShipMeshComponent;
    ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    ShipMeshComponent->SetStaticMesh(ShipMesh.Object);

    // Cache our sound effect
    static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
    FireSound = FireAudio.Object;

    // Create a camera boom...
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->SetUsingAbsoluteRotation(true);  // Don't want arm to rotate when ship does
    CameraBoom->TargetArmLength = 1200.f;
    CameraBoom->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
    CameraBoom->bDoCollisionTest = false;  // Don't want to pull camera in when it collides with level

    // Create a camera...
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    CameraComponent->bUsePawnControlRotation = false;  // Camera does not rotate relative to arm

    // Movement
    MoveSpeed = 1000.0f;
    // Weapon
    GunOffset = FVector(90.f, 0.f, 0.f);
    FireRate = 0.1f;
    bCanFire = true;
}

void ATGTestPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
    check(PlayerInputComponent);

    // set up gameplay key bindings
    PlayerInputComponent->BindAxis(MoveForwardBinding);
    PlayerInputComponent->BindAxis(MoveRightBinding);
    PlayerInputComponent->BindAxis(FireForwardBinding);
    PlayerInputComponent->BindAxis(FireRightBinding);
}

struct TestTask : public TaskAnt::AntTask {
    int m_outputNum;
    int m_time;
    TestTask(string name, const int& outputNum) : AntTask(name), m_outputNum(outputNum) {}
    virtual ~TestTask() override {}
    virtual void Run() override {
        for (int i = 0; i < m_outputNum; i++) {
            m_time++;
            for (int j = 0; j < m_outputNum * 1000; j++)
                m_time += sqrt(j);
        }
    }
};

void ATGTestPawn::BeginPlay() {
    Super::BeginPlay();
    imnodes::Initialize();
    UE_LOG(LogTemp, Warning, TEXT("cnmd Begin"));
}

void ATGTestPawn::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    Super::EndPlay(EndPlayReason);
    imnodes::Shutdown();
    UE_LOG(LogTemp, Warning, TEXT("cnmd End"));
}

void ATGTestPawn::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);

    static int frameNum = 0;
    frameNum++;
#if WITH_IMGUI
    UE_LOG(LogTemp, Warning, TEXT("cnmd Tick imgui"));
    // 启动若干任务
    auto event1 = TaskAnt::AntManager::GetInstance()->ScheduleTask(frameNum, new TestTask("Task 1", 2), vector<shared_ptr<TaskAnt::AntEvent>>{});
    auto event2 = TaskAnt::AntManager::GetInstance()->ScheduleTask(frameNum, new TestTask("Task 2", 4), vector<shared_ptr<TaskAnt::AntEvent>>{});
	auto event3 = TaskAnt::AntManager::GetInstance()->ScheduleTask(frameNum, new TestTask("Task 3", 3), vector<shared_ptr<TaskAnt::AntEvent>>{event1, event2});
    // auto event4 = TaskAnt::AntManager::GetInstance()->ScheduleTask(frameNum, new TestTask("Task 4", 2), vector<shared_ptr<TaskAnt::AntEvent>>{event3});
    // auto event5 = TaskAnt::AntManager::GetInstance()->ScheduleTask(frameNum, new TestTask("Task 5", 8), vector<shared_ptr<TaskAnt::AntEvent>>{event1, event2});
    // auto event6 = TaskAnt::AntManager::GetInstance()->ScheduleTask(frameNum, new TestTask("Task 6", 3), vector<shared_ptr<TaskAnt::AntEvent>>{event2});
    // auto event7 = TaskAnt::AntManager::GetInstance()->ScheduleTask(frameNum, new TestTask("Task 7", 5), vector<shared_ptr<TaskAnt::AntEvent>>{event5});
    // auto event8 = TaskAnt::AntManager::GetInstance()->ScheduleTask(frameNum, new TestTask("Task 8", 9), vector<shared_ptr<TaskAnt::AntEvent>>{});
    // auto event9 = TaskAnt::AntManager::GetInstance()->ScheduleTask(frameNum, new TestTask("Task 9", 7), vector<shared_ptr<TaskAnt::AntEvent>>{event4, event8});
    // auto event10 = TaskAnt::AntManager::GetInstance()->ScheduleTask(frameNum, new TestTask("Task 10", 9), vector<shared_ptr<TaskAnt::AntEvent>>{event6});
    // auto event11 = TaskAnt::AntManager::GetInstance()->ScheduleTask(frameNum, new TestTask("Task 11", 9), vector<shared_ptr<TaskAnt::AntEvent>>{event4, event5, event6, event7, event9, event10});

    // 完成任务
	event3->Complete();

	TaskAnt::AntWatcher::GetInstance()->ImGuiRenderTick();
#endif

    // Find movement direction
    const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
    const float RightValue = GetInputAxisValue(MoveRightBinding);

    // Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
    const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

    // Calculate  movement
    const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

    // If non-zero size, move this actor
    if (Movement.SizeSquared() > 0.0f) {
        const FRotator NewRotation = Movement.Rotation();
        FHitResult Hit(1.f);
        RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);

        if (Hit.IsValidBlockingHit()) {
            const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
            const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
            RootComponent->MoveComponent(Deflection, NewRotation, true);
        }
    }

    // Create fire direction vector
    const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
    const float FireRightValue = GetInputAxisValue(FireRightBinding);
    const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.f);

    // Try and fire a shot
    FireShot(FireDirection);
}

void ATGTestPawn::FireShot(FVector FireDirection) {
    // If it's ok to fire again
    if (bCanFire == true) {
        // If we are pressing fire stick in a direction
        if (FireDirection.SizeSquared() > 0.0f) {
            const FRotator FireRotation = FireDirection.Rotation();
            // Spawn projectile at an offset from this pawn
            const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

            UWorld* const World = GetWorld();
            if (World != NULL) {
                // spawn the projectile
                World->SpawnActor<ATGTestProjectile>(SpawnLocation, FireRotation);
            }

            bCanFire = false;
            World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ATGTestPawn::ShotTimerExpired, FireRate);

            // try and play the sound if specified
            if (FireSound != nullptr) {
                UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
            }

            bCanFire = false;
        }
    }
}

void ATGTestPawn::ShotTimerExpired() {
    bCanFire = true;
}
