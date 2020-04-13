#include "Bullet.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

ABullet::ABullet() {
    static ConstructorHelpers::FObjectFinder<UStaticMesh> bulletMesh(TEXT("/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile"));

    // Mesh
    m_bulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh0"));
    RootComponent = m_bulletMesh;
    m_bulletMesh->SetStaticMesh(bulletMesh.Object);
    m_bulletMesh->BodyInstance.SetCollisionProfileName("Bullet");
    m_bulletMesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

    // Bullet Movement
    m_bulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement0"));
    m_bulletMovement->UpdatedComponent = m_bulletMesh;
    m_bulletMovement->InitialSpeed = 1500.f;
    m_bulletMovement->MaxSpeed = 1500.f;
    m_bulletMovement->bRotationFollowsVelocity = true;
    m_bulletMovement->bShouldBounce = false;
    m_bulletMovement->ProjectileGravityScale = 0.f;

    // Life Span
    InitialLifeSpan = 3.f;
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
    if ((OtherActor != NULL) && (OtherComp != NULL) && (OtherActor != this)) {
        OtherActor->TakeDamage(12.f, FDamageEvent(), NULL, NULL);
    }
    Destroy();
}
