#include "Bullet.h"

#include "Engine/StaticMesh.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ABullet::ABullet()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> bulletMesh(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));

	// 创建网格组件
	m_bulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh0"));
	RootComponent = m_bulletMesh;
	m_bulletMesh->SetStaticMesh(bulletMesh.Object);
	m_bulletMesh->BodyInstance.SetCollisionProfileName("Bullet");
	m_bulletMesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

	// 子弹行为
	m_bulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement0"));
	m_bulletMovement->UpdatedComponent = m_bulletMesh;
	m_bulletMovement->InitialSpeed = 3000.f;
	m_bulletMovement->MaxSpeed = 3000.f;
	m_bulletMovement->bRotationFollowsVelocity = true;
	m_bulletMovement->bShouldBounce = false;
	m_bulletMovement->ProjectileGravityScale = 0.f;

	// 自动销毁
	InitialLifeSpan = 3.f;
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if ((OtherActor != NULL) && (OtherComp != NULL) && (OtherActor != this)) {
		OtherActor->TakeDamage(12.f, FDamageEvent(), NULL, NULL);
	}
	Destroy();
}