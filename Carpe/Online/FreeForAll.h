// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreGameModeBase.h"
#include "FreeForAll.generated.h"

class APlayerState;

UCLASS()
class AFreeForAll : public ACoreGameModeBase
{
	GENERATED_UCLASS_BODY()

protected:

	/** best player */
	UPROPERTY(transient)
	APlayerState* WinnerPlayerState;

	/** check who won */
	virtual void DetermineMatchWinner() override;

	/** check if PlayerState is a winner */
	virtual bool IsWinner(AGamePlayerState* PlayerState) const override;
};
