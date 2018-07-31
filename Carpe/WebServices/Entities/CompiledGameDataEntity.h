#pragma once

#include "GameItemEntity.h"
#include "GameItemAttributeEntity.h"
#include "NewsFeedEntity.h"
#include "GameSaleEntity.h"
#include "GameSettingEntity.h"
#include "BundleEntity.h"
#include "BundleItemEntity.h"
#include "GameCurrencyConversionEntity.h"
#include "IapPackageEntity.h"
#include "PatchEntity.h"
#include "AchievementEntity.h"
#include "LeaderBoardEntity.h"

#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "CompiledGameDataEntity.generated.h"

USTRUCT()
struct FCompiledGameDataEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<FGameItemEntity> GameItems;

	UPROPERTY()
	TArray<FGameItemAttributeEntity> GameItemAttributes;

	UPROPERTY()
	TArray<FNewsFeedEntity> NewsFeeds;

	UPROPERTY()
	TArray<FGameSaleEntity> GameSales;

	UPROPERTY()
	TArray<FGameSettingEntity> GameSettings;

	UPROPERTY()
	TArray<FBundleEntity> Bundles;

	UPROPERTY()
	TArray<FBundleItemEntity> BundleItems;

	UPROPERTY()
	TArray<FGameCurrencyConversionEntity> GameCurrencyConversions;

	UPROPERTY()
	TArray<FIapPackageEntity> IapPackages;

	UPROPERTY()
	TArray<FPatchEntity> Patches;

	UPROPERTY()
	TArray<FAchievementEntity> Achievements;

	UPROPERTY()
	TArray<FLeaderBoardEntity> Leaderboards;

	UPROPERTY()
	TArray<FString> CustomGameData;

	UPROPERTY()
	int32 MethodType = 0;

	UPROPERTY()
	FString Catalog = "";

	UPROPERTY()
	int32 UserId = 0;

	UPROPERTY()
	FString DeviceSerial = "";

	UPROPERTY()
	int32 Version = 0;

	UPROPERTY()
	FString EntityName = "";


	FCompiledGameDataEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "CompiledGameDataEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"GameItems\": 1,\"GameItemAttributes\": 1,\"NewsFeeds\": 1,\"GameSales\": 1,\"GameSettings\": 1,\"Bundles\": 1,\"BundleItems\": 1,\"GameCurrencyConversions\": 1,\"IapPackages\": 1,\"Patches\": 1,\"Achievements\": 1,\"Leaderboards\": 1,\"CustomGameData\": 1,\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FCompiledGameDataEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FCompiledGameDataEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
