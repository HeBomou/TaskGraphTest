#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestBullet.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class TGTEST_API ATestBullet : public AActor {
    GENERATED_BODY()
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* m_bulletMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
    UProjectileMovementComponent* m_bulletMovement;

   public:
    ATestBullet();

   public:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
