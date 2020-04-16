#include "TankManagerTG.h"

#include "Async/ParallelFor.h"
#include "BulletTG.h"
#include "Engine/World.h"
#include "TankTG.h"

#include "../TaskAnt/AntEvent.h"
#include "../TaskAnt/AntManager.h"
#include "../TaskAnt/AntTask.h"
#include "../TaskAnt/AntWatcher.h"

ATankManagerTG::ATankManagerTG() {
    PrimaryActorTick.bCanEverTick = true;

    // Scene Component
    m_scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene0"));
    RootComponent = m_scene;

    // Spawn
    m_spawnRecoverTime = 5;
    m_spawnTimer = 0;

	TaskAnt::AntManager::GetInstance(); // TODO: 线程有一定概率不可用，可能创建失败或者被挂起，也许是TaskGraph已经开了太多线程
}

void ATankManagerTG::BeginPlay() {
	Super::BeginPlay();

	imnodes::Initialize();
}

void ATankManagerTG::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	imnodes::Shutdown();
}

struct TestGameLogicTask : public TaskAnt::AntTask {
	int m_time;
	const int m_begin;
	const int m_end;
	const TArray<ATankTG*>& m_tanks;
	TArray<FVector> &m_moveDirs;
	TArray<FVector> &m_shootDirs;
	TestGameLogicTask(string name, const int& begin, const int& end, const TArray<ATankTG*>& tanks, TArray<FVector> &moveDirs, TArray<FVector> &shootDirs) : AntTask(name), m_begin(begin), m_end(end), m_tanks(tanks), m_moveDirs(moveDirs), m_shootDirs(shootDirs) {}
	virtual ~TestGameLogicTask() override {}
	virtual void Run() override {
		for (int i = m_begin; i < m_end; i++) {
			ATankTG* enemy = NULL;
			float mnDis = FLT_MAX;
			for (int32 j = 0; j < m_tanks.Num(); j++) {
				if (i == j) continue;

				// Manually increase complexity
				float stupidSum = 0;
				for (int32 t = 0; t < 2000; t++)
					stupidSum += FMath::Sqrt(t);

				auto dis = stupidSum + (m_tanks[j]->GetActorLocation() - m_tanks[i]->GetActorLocation()).SizeSquared();
				if (m_tanks[j]->m_teamId != m_tanks[i]->m_teamId && dis < mnDis)
					enemy = m_tanks[j], mnDis = dis;
			}
			if (enemy)
				m_moveDirs[i] = m_shootDirs[i] = (enemy->GetActorLocation() - m_tanks[i]->GetActorLocation()).GetSafeNormal();
			else {
				m_shootDirs[i] = FVector();
				while (m_moveDirs[i].SizeSquared() == 0)
					m_moveDirs[i] = FVector(FMath::RandPointInCircle(1).GetSafeNormal(), 0);
			}
		}
	}
};

template<typename ... Args>
string string_format(const string& format, Args ... args) {
	size_t size = 1 + snprintf(nullptr, 0, format.c_str(), args ...);  // Extra space for \0
	// unique_ptr<char[]> buf(new char[size]);
	char *bytes = new char[size];
	snprintf(bytes, size, format.c_str(), args ...);
	string res(bytes);
	delete[] bytes;
	return res;
}

void ATankManagerTG::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

	static int frameNum = 0;
	frameNum++;


	UWorld* world = GetWorld();
	if (!world) return;

	// Tanks Compute
	TArray<ATankTG*> tanks;
	for (auto it = m_tanks.CreateIterator(); it; ++it)
		tanks.Add(*it);
	TArray<FVector> shootDirs;
	TArray<FVector> moveDirs;
	shootDirs.Init(FVector(), tanks.Num());
	moveDirs.Init(FVector(), tanks.Num());

	// 游戏逻辑的并行
	// TODO: 应当使用模板来封装ScheduleParallelFor
	int chunkNum = 32;
	vector<shared_ptr<TaskAnt::AntEvent>> evts;
	for (int i = 0; i < tanks.Num(); i += chunkNum)
		evts.emplace_back(TaskAnt::AntManager::GetInstance()->ScheduleTask(frameNum, new TestGameLogicTask(string_format("Game Logic Task %d", i / chunkNum), i, min(i + chunkNum, tanks.Num()), tanks, moveDirs, shootDirs), vector<shared_ptr<TaskAnt::AntEvent>>{}));
	// 任务验收
	for (auto evt : evts)
		evt->Complete();

	for (int i = 0; i < tanks.Num(); i++) {
		auto tank = tanks[i];
		// Move
		auto moveDir = moveDirs[i];
		if (tank->m_moveTimer < 0)
			tank->m_moveTimer += tank->m_moveRecoverTime, tank->m_moveDir = moveDir, tank->m_moveRot = moveDir.Rotation();
		else
			tank->m_moveTimer -= DeltaTime;
		tank->Move();

		// Shoot
		auto shootDir = shootDirs[i];
		if (tank->m_shootTimer < 0) {
			tank->m_shootTimer += tank->m_shootRecoverTime;
			if (shootDir.SizeSquared() > 0)
				world->SpawnActor<ABulletTG>(tank->GetActorLocation() + shootDir * 80.f, shootDir.Rotation());
		}
		else
			tank->m_shootTimer -= DeltaTime;
	}

	// Kill Tanks
	TArray<ATankTG*> tanksToRemove;
	for (auto it = m_tanks.CreateIterator(); it; ++it) {
		if ((*it)->m_health <= 0)
			tanksToRemove.Add(*it);
	}
	for (int i = 0; i < tanksToRemove.Num(); i++) {
		m_tanks.Remove(tanksToRemove[i]);
		tanksToRemove[i]->Destroy();
	}

	// Spawn
	if (m_spawnTimer < 0) {
		m_spawnTimer += m_spawnRecoverTime;
		for (uint32 i = 0; i < 40; i++) {
			auto tank = world->SpawnActor<ATankTG>(FVector(FMath::RandPointInCircle(1500), 0) + GetActorLocation(), FRotator());
			if (tank)
				m_tanks.Add(tank);
		}
	}
	else
		m_spawnTimer -= DeltaTime;

	// 渲染依赖图
	TaskAnt::AntWatcher::GetInstance()->ImGuiRenderTick();
}
