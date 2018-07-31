// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Runtime/Engine/Classes/Engine/GameEngine.h"
#include "CoreGameEngine.generated.h"

UCLASS()
class CARPE_API UCoreGameEngine : public UGameEngine
{
	GENERATED_UCLASS_BODY()

public:
	/* Hook up specific callbacks */
	virtual void Init(IEngineLoop* InEngineLoop);

	/**
	 * 	All regular engine handling, plus update ShooterKing state appropriately.
	 */
	virtual void HandleNetworkFailure(UWorld *World, UNetDriver *NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString) override;
};

