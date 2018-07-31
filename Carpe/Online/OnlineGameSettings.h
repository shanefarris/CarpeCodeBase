// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

/**
 * General session settings for a Shooter game
 */
class FGameOnlineSessionSettings : public FOnlineSessionSettings
{
public:

	FGameOnlineSessionSettings(bool bIsLAN = false, bool bIsPresence = false, int32 MaxNumPlayers = 4);
	virtual ~FGameOnlineSessionSettings() {}
};

/**
 * General search setting for a Shooter game
 */
class FGameOnlineSearchSettings : public FOnlineSessionSearch
{
public:
	FGameOnlineSearchSettings(bool bSearchingLAN = false, bool bSearchingPresence = false);

	virtual ~FGameOnlineSearchSettings() {}
};

/**
 * Search settings for an empty dedicated server to host a match
 */
class FGameOnlineSearchSettingsEmptyDedicated : public FGameOnlineSearchSettings
{
public:
	FGameOnlineSearchSettingsEmptyDedicated(bool bSearchingLAN = false, bool bSearchingPresence = false);

	virtual ~FGameOnlineSearchSettingsEmptyDedicated() {}
};
