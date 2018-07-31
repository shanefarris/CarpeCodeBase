// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PersistentUser.h"
#include "GameLocalPlayer.generated.h"

UCLASS(config=Engine, transient)
class UGameLocalPlayer : public ULocalPlayer
{
	GENERATED_UCLASS_BODY()

public:

	virtual void SetControllerId(int32 NewControllerId) override;

	virtual FString GetNickname() const;

	class UPersistentUser* GetPersistentUser() const;
	
	/** Initializes the PersistentUser */
	void LoadPersistentUser();

private:
	/** Persistent user data stored between sessions (i.e. the user's savegame) */
	UPROPERTY()
	class UPersistentUser* PersistentUser;
};



