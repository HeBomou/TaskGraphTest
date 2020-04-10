#include "Bullet.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

ABullet::ABullet()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> bulletMesh(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));

	// Create mesh component for the projectile sphere
	m_bulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh0"));
	RootComponent = m_bulletMesh;
	m_bulletMesh->SetStaticMesh(bulletMesh.Object);
	m_bulletMesh->SetupAttachment(RootComponent);
	m_bulletMesh->BodyInstance.SetCollisionProfileName("Bullet");
	m_bulletMesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit); // 碰撞事件

	// 子弹
	m_bulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement0"));
	m_bulletMovement->UpdatedComponent = m_bulletMesh;
	m_bulletMovement->InitialSpeed = 3000.f;
	m_bulletMovement->MaxSpeed = 3000.f;
	m_bulletMovement->bRotationFollowsVelocity = true;
	m_bulletMovement->bShouldBounce = false;
	m_bulletMovement->ProjectileGravityScale = 0.f; // 无重力

	// 三秒自动销毁
	InitialLifeSpan = 3.f;
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("cnm"));
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		OtherActor->TakeDamage(12.f, FDamageEvent(), NULL, NULL);
	}
	Destroy();
}
