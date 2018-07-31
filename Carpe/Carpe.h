// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
//#include "CoreMinimal.h"
#include "ModuleInterface.h"

#include "Containers/Array.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "ParticleDefinitions.h"
#include "SoundDefinitions.h"
//#include "Carpe/Online/CoreGameModeBase.h"
//#include "Carpe/Online/CoreGameState.h"
//#include "Carpe/Player/CharacterBase.h"
//#include "Carpe/Player/CharacterMovement.h"
//#include "Carpe/Player/GamePlayerController.h"

#include "CarpeClasses.h"

#define PB PROPERTY()
#define PS UPROPERTY(EditDefaultsOnly, Category = "CFG")
#define PR UPROPERTY(EditDefaultsOnly, Category = "CFG|AARequired")

#define COLLISION_WEAPON		ECC_GameTraceChannel1
#define COLLISION_PROJECTILE2	ECC_GameTraceChannel2
#define COLLISION_PICKUP2		ECC_GameTraceChannel3

#define MAX_PLAYER_NAME_LENGTH 16


/** Set to 1 to pretend we're building for console even on a PC, for testing purposes */
#define SHOOTER_SIMULATE_CONSOLE_UI	0

#if PLATFORM_PS4 || PLATFORM_XBOXONE || SHOOTER_SIMULATE_CONSOLE_UI
#define SHOOTER_CONSOLE_UI 1
#else
#define SHOOTER_CONSOLE_UI 0
#endif


class ICoreFrameworkModule : public IModuleInterface
{
public:
	virtual void StartInGameLoadingScreen() = 0;
};



