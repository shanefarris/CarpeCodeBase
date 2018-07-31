#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "UserAchievementEntity.generated.h"

USTRUCT()
struct FUserAchievementEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 UserAchievementId = 0;

	UPROPERTY()
	int32 AchievementId = 0;

	UPROPERTY()
	int32 GameId = 0;

	UPROPERTY()
	FString CreateDate = "";

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


	FUserAchievementEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "UserAchievementEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"UserAchievementId\": 1,\"AchievementId\": 1,\"GameId\": 1,\"CreateDate\":\"createdate\",\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FUserAchievementEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FUserAchievementEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("UserAchievementId: %d"), data[0].UserAchievementId);
		UE_LOG(LogCore, Log, TEXT("AchievementId: %d"), data[0].AchievementId);
		UE_LOG(LogCore, Log, TEXT("GameId: %d"), data[0].GameId);
		UE_LOG(LogCore, Log, TEXT("CreateDate: %s"), *data[0].CreateDate);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
