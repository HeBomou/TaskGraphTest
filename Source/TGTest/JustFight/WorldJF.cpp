#include "WorldJF.h"

#include <memory>
#include <vector>

#include "../ImGuiCommon.h"
#include "../TaskAnt/AntManager.h"
#include "../TaskAnt/AntWatcher.h"
#include "../imnodes/imnodes.h"
#include "Engine/World.h"
#include "ImGuiModule.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "TestEnemy.h"

AWorldJF::AWorldJF() {
    PrimaryActorTick.bCanEverTick = true;

    // Root cmpt
    m_scene = CreateDefaultSubobject<USceneComponent>(TEXT("WorldJF"));
    RootComponent = m_scene;

    // Spawn
    m_spawnRecoverTime = 50;
    m_spawnTimer = 0;
}

void AWorldJF::BeginPlay() {
    Super::BeginPlay();

    // Show mouse cursor
    APlayerController* playerController = UGameplayStatics::GetPlayerController(GWorld, 0);
    playerController->bShowMouseCursor = true;

    // ImGui enable input
    FImGuiModule::Get().GetProperties().SetInputEnabled(true);

    // imnodes init
    imnodes::Initialize();
}

void AWorldJF::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    Super::EndPlay(EndPlayReason);

    // imnodes shutdown
    imnodes::Shutdown();
}

void AWorldJF::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    static int frameNum = 0;
    frameNum++;

    UWorld* world = GetWorld();
    if (!world) return;

    // MyTaskTick
    TArray<ATestEnemy*> tanks;
    for (auto it = m_tanks.CreateIterator(); it; ++it)
        tanks.Add(*it);
    auto evts = TaskAnt::AntManager::GetInstance()->ScheduleTaskParallel(
        frameNum, "Enemy Tick", tanks.Num(), [&tanks, DeltaTime](int i) { tanks[i]->MyTaskTick(DeltaTime); }, std::vector<std::shared_ptr<TaskAnt::AntEvent>>());
    for (auto evt : evts)
        evt->Complete();
    // MyTick
    for (auto tank : tanks) {
        tank->MyTick(DeltaTime);
		if (tank->m_health <= 0)
			m_tanks.Remove(tank);
	}

    // Spawn
    if (m_spawnTimer < 0) {
        m_spawnTimer += m_spawnRecoverTime;
        for (uint32 i = 0; i < 40; i++) {
            auto tank = world->SpawnActor<ATestEnemy>(FVector(FMath::RandPointInCircle(1500), 0) + GetActorLocation(), FRotator());
            if (tank)
                m_tanks.Add(tank);
        }
    } else
        m_spawnTimer -= DeltaTime;

    // Render ImGui
    TaskAnt::AntWatcher::GetInstance()->Tick();
}
