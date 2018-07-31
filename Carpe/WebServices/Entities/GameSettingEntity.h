#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "GameSettingEntity.generated.h"

USTRUCT()
struct FGameSettingEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 GameSettingId = 0;

	UPROPERTY()
	int32 GameSettingCategoryId = 0;

	UPROPERTY()
	FString Description = "";

	UPROPERTY()
	int32 GameId = 0;

	UPROPERTY()
	FString LabelName = "";

	UPROPERTY()
	FString Value = "";

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


	FGameSettingEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "GameSettingEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"GameSettingId\": 1,\"GameSettingCategoryId\": 1,\"Description\":\"description\",\"GameId\": 1,\"LabelName\":\"labelname\",\"Value\":\"value\",\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FGameSettingEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FGameSettingEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("GameSettingId: %d"), data[0].GameSettingId);
		UE_LOG(LogCore, Log, TEXT("GameSettingCategoryId: %d"), data[0].GameSettingCategoryId);
		UE_LOG(LogCore, Log, TEXT("Description: %s"), *data[0].Description);
		UE_LOG(LogCore, Log, TEXT("GameId: %d"), data[0].GameId);
		UE_LOG(LogCore, Log, TEXT("LabelName: %s"), *data[0].LabelName);
		UE_LOG(LogCore, Log, TEXT("Value: %s"), *data[0].Value);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
