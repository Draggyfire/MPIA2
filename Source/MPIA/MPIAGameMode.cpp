// Copyright Epic Games, Inc. All Rights Reserved.

#include "MPIAGameMode.h"
#include "MPIACharacter.h"
#include "UObject/ConstructorHelpers.h"

AMPIAGameMode::AMPIAGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
