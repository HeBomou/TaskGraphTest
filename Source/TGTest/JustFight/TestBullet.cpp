#include "TestBullet.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

ATestBullet::ATestBullet()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> bulletMesh(TEXT("/Game/JustFight/Berserker_BerserkerBullet.Berserker_BerserkerBullet"));

    // Root cmpt for rotation
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Mesh
    m_bulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletTGMesh0"));
    m_bulletMesh->SetStaticMesh(bulletMesh.Object);
    m_bulletMesh->BodyInstance.SetCollisionProfileName("BulletTG");
    m_bulletMesh->OnComponentHit.AddDynamic(this, &ATestBullet::OnHit);
    m_bulletMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
    m_bulletMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    // Bullet Movement
    m_bulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletTGMovement0"));
    m_bulletMovement->UpdatedComponent = RootComponent;
    m_bulletMovement->InitialSpeed = 3000.f;
    m_bulletMovement->MaxSpeed = 3000.f;
    m_bulletMovement->bRotationFollowsVelocity = true;
    m_bulletMovement->bShouldBounce = false;
    m_bulletMovement->ProjectileGravityScale = 0.f;

    // Life Span
    InitialLifeSpan = 3.f;
}

void ATestBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
    if ((OtherActor != NULL) && (OtherComp != NULL) && (OtherActor != this)) {
        OtherActor->TakeDamage(50.f, FDamageEvent(), NULL, NULL);
    }
    Destroy();
}