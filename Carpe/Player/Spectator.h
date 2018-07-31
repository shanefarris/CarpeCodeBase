// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Spectator.generated.h"

// TODO class SShooterDemoHUD;

UCLASS(config=Game)
class ASpectator : public APlayerController
{
	GENERATED_UCLASS_BODY()

public:
	virtual void SetupInputComponent() override;
	virtual void SetPlayer( UPlayer* Player ) override;
	virtual void Destroyed() override;

	void OnToggleInGameMenu();
	void OnIncreasePlaybackSpeed();
	void OnDecreasePlaybackSpeed();

	int32 PlaybackSpeed;

};

