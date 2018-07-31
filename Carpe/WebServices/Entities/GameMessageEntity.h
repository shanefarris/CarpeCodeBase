#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "GameMessageEntity.generated.h"

USTRUCT()
struct FGameMessageEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Subject = "";

	UPROPERTY()
	FString Body = "";

	UPROPERTY()
	FString To = "";

	UPROPERTY()
	FString From = "";

	UPROPERTY()
	int32 GameId = 0;

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


	FGameMessageEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "GameMessageEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"Subject\":\"subject\",\"Body\":\"body\",\"To\":\"to\",\"From\":\"from\",\"GameId\": 1,\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FGameMessageEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FGameMessageEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("Subject: %s"), *data[0].Subject);
		UE_LOG(LogCore, Log, TEXT("Body: %s"), *data[0].Body);
		UE_LOG(LogCore, Log, TEXT("To: %s"), *data[0].To);
		UE_LOG(LogCore, Log, TEXT("From: %s"), *data[0].From);
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
