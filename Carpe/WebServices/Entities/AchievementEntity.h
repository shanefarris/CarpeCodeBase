#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "AchievementEntity.generated.h"

USTRUCT()
struct FAchievementEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 AchievementId = 0;

	UPROPERTY()
	int32 GameId = 0;

	UPROPERTY()
	FString Title = "";

	UPROPERTY()
	int32 Value = 0;

	UPROPERTY()
	FString ValueString = "";

	UPROPERTY()
	FString LabelName = "";

	UPROPERTY()
	bool IsHidden;

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


	FAchievementEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "AchievementEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"AchievementId\": 1,\"GameId\": 1,\"Title\":\"title\",\"Value\": 1,\"ValueString\":\"valuestring\",\"LabelName\":\"labelname\",\"IsHidden\": 1,\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FAchievementEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FAchievementEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("AchievementId: %d"), data[0].AchievementId);
		UE_LOG(LogCore, Log, TEXT("GameId: %d"), data[0].GameId);
		UE_LOG(LogCore, Log, TEXT("Title: %s"), *data[0].Title);
		UE_LOG(LogCore, Log, TEXT("Value: %d"), data[0].Value);
		UE_LOG(LogCore, Log, TEXT("ValueString: %s"), *data[0].ValueString);
		UE_LOG(LogCore, Log, TEXT("LabelName: %s"), *data[0].LabelName);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
