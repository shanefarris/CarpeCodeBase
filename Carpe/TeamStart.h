// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Runtime/Engine/Classes/GameFramework/PlayerStart.h"
#include "TeamStart.generated.h"

UCLASS()
class CARPE_API ATeamStart : public APlayerStart
{
	GENERATED_UCLASS_BODY()

public:
	/** Which team can start at this point */
	UPROPERTY(EditInstanceOnly, Category="CFG|Team")
	int32 SpawnTeam;

	/** Whether players can start at this point */
	UPROPERTY(EditInstanceOnly, Category= "CFG|Team")
	uint32 bNotForPlayers:1;

	/** Whether bots can start at this point */
	UPROPERTY(EditInstanceOnly, Category= "CFG|Team")
	uint32 bNotForBots:1;
};
