#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TankManager.generated.h"

UCLASS()
class TASKGRAPHTEST_API ATankManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ATankManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void RecoverSpawn();

private:
	bool bCanSpawn;

};
