#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class UStaticMeshComponent;

UCLASS()
class TGTEST_API ATank : public APawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_tankMesh;

public:
	ATank();


public:
	virtual void Tick(float DeltaTime) override;

private:
	float m_shootRecoverTime;
	float m_shootTimer;
	inline void TryShoot(FVector shootDir, float dT);
};
