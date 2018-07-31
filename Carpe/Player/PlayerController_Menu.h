// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Types.h"
#include "PlayerController_Menu.generated.h"

UCLASS()
class APlayerController_Menu : public APlayerController
{
	GENERATED_UCLASS_BODY()

public:
	/** After game is initialized */
	virtual void PostInitializeComponents() override;
};

