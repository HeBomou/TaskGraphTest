// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TGTestGameMode.h"
#include "TGTestPawn.h"

ATGTestGameMode::ATGTestGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ATGTestPawn::StaticClass();
}

