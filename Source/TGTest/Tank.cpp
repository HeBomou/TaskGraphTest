#include "Tank.h"

#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"

#include "Bullet.h"

ATank::ATank()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tankMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));

	// 网格组件
	m_tankMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankMesh0"));
	RootComponent = m_tankMesh;
	m_tankMesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	m_tankMesh->SetStaticMesh(tankMesh.Object);

	// 射击间隔
	m_shootRecoverTime = 0.3f;
	m_shootTimer = 0;
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// TODO: 寻找目标

	TryShoot(GetActorForwardVector(), DeltaTime);
}

void ATank::TryShoot(FVector shootDir, float dT)
{
	if (m_shootTimer < 0) {
		m_shootTimer += m_shootRecoverTime;
		UWorld* world = GetWorld();
		if (world)
			world->SpawnActor<ABullet>(GetActorLocation() + shootDir * 90.f, shootDir.Rotation());
	}
	else m_shootTimer -= dT;
}
