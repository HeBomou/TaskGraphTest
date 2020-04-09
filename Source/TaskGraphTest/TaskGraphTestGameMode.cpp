// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TaskGraphTestGameMode.h"
#include "TaskGraphTestPawn.h"

ATaskGraphTestGameMode::ATaskGraphTestGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ATaskGraphTestPawn::StaticClass();
}

