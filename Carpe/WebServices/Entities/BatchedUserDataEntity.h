#pragma once

#include "BuddyRequestEntity.h"
#include "UserEconomyEntity.h"
#include "UserCustomDataEntity.h"
#include "UserAchievementEntity.h"
#include "BuddyListEntity.h"
#include "UserLeaderBoardEntity.h"
#include "UserGameItemEntity.h"
#include "SaleOfferEntity.h"
#include "ClanEntity.h"
#include "ClanEntity.h"
#include "MessageEntity.h"
#include "MessageEntity.h"

#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "BatchedUserDataEntity.generated.h"

USTRUCT()
struct FBatchedUserDataEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 GameId = 0;

	UPROPERTY()
	TArray<FBuddyRequestEntity> BuddyRequests;

	UPROPERTY()
	FUserEconomyEntity UserEconomy;

	UPROPERTY()
	FUserCustomDataEntity UserCustomData;

	UPROPERTY()
	TArray<FUserAchievementEntity> UserAchievements;

	UPROPERTY()
	TArray<FBuddyListEntity> BuddyLists;

	UPROPERTY()
	TArray<FUserLeaderBoardEntity> UserLeaderboards;

	UPROPERTY()
	TArray<FUserGameItemEntity> UserGameItems;

	UPROPERTY()
	TArray<FSaleOfferEntity> SaleOffers;

	UPROPERTY()
	TArray<FClanEntity> Clans;

	UPROPERTY()
	TArray<FClanEntity> ClanInvites;

	UPROPERTY()
	TArray<FMessageEntity> Messages;

	UPROPERTY()
	TArray<FMessageEntity> MessageBans;

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


	FBatchedUserDataEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "BatchedUserDataEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"GameId\": 1,\"BuddyRequests\": 1,\"UserEconomy\": 1,\"UserCustomData\": 1,\"UserAchievements\": 1,\"BuddyLists\": 1,\"UserLeaderboards\": 1,\"UserGameItems\": 1,\"SaleOffers\": 1,\"Clans\": 1,\"ClanInvites\": 1,\"Messages\": 1,\"MessageBans\": 1,\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FBatchedUserDataEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FBatchedUserDataEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("GameId: %d"), data[0].GameId);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
