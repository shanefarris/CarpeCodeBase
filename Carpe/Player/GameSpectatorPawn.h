// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Runtime/Engine/Classes/GameFramework/SpectatorPawn.h"
#include "GameSpectatorPawn.generated.h"


UCLASS(config = Game, Blueprintable, BlueprintType)
class AGameSpectatorPawn : public ASpectatorPawn
{
	GENERATED_UCLASS_BODY()

public:
	// Begin ASpectatorPawn overrides
	/** Overridden to implement Key Bindings the match the player controls */
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End Pawn overrides
	
	// Frame rate linked look
	void LookUpAtRate(float Val);
};
