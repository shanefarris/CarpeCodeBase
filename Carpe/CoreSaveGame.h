// Copyright 2015 Corn Fed Games LLC

#pragma once

#include "GameFramework/SaveGame.h"
#include "CoreSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CARPE_API UCoreSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = Save)
	FString PlayerName;
	
	UPROPERTY(VisibleAnywhere, Category = Save)
	FVector PlayerPosition;

	UPROPERTY(VisibleAnywhere, Category = Save)
	FVector PlayerLookAt;

	UPROPERTY(VisibleAnywhere, Category = Save)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Save)
	uint32 UserIndex;

	UCoreSaveGame();

	static void SaveGame();

	static void LoadGame();
};
