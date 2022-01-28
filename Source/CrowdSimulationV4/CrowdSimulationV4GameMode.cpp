// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrowdSimulationV4GameMode.h"
#include "CrowdSimulationV4Character.h"
#include "UObject/ConstructorHelpers.h"

ACrowdSimulationV4GameMode::ACrowdSimulationV4GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
