// Copyright Epic Games, Inc. All Rights Reserved.

#include "DroneWarGameMode.h"
#include "DroneWarCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "DronePawn.h"

ADroneWarGameMode::ADroneWarGameMode()
{
	DefaultPawnClass = nullptr;
	
	//// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	//DefaultPawnClass = PlayerPawnBPClass.Class;
	//	
	//}

	static ConstructorHelpers::FClassFinder<APawn> DronePawnBPClass(TEXT("/Game/Drone/BP_DronePawn"));
	if (DronePawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = DronePawnBPClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("Drone Pawn load Success"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Drone Pawn load Fail"));
	}


}
