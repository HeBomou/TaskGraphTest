// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TankFinder.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TGTEST_API UTankFinder : public UObject
{
	GENERATED_BODY()

public:
	UTankFinder(const FObjectInitializer &ObjectInitializer);
	// static UTankFinder *GetInstance()
	// {
	// 	if (!s_instance)
	// 		s_instance = 
	// }

private:
	// UTankFinder *s_instance;
};
