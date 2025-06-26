// Copyright Epic Games, Inc. All Rights Reserved.

#include "DroneWarGameMode.h"
#include "DroneWarCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADroneWarGameMode::ADroneWarGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
