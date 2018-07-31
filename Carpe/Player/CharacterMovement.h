// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

/**
 * Movement component meant for use with Pawns.
 */

#pragma once
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "CharacterMovement.generated.h"

UCLASS()
class UCharacterMovement : public UCharacterMovementComponent
{
	GENERATED_UCLASS_BODY()

public:
	virtual float GetMaxSpeed() const override;
};

