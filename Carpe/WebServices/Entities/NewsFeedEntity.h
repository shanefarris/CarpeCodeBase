#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "NewsFeedEntity.generated.h"

USTRUCT()
struct FNewsFeedEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 NewsFeedId = 0;

	UPROPERTY()
	int32 NewsFeedTypeId = 0;

	UPROPERTY()
	int32 NewsFeedFormatTypeId = 0;

	UPROPERTY()
	FString Title = "";

	UPROPERTY()
	FString CreateDate = "";

	UPROPERTY()
	FString Body = "";

	UPROPERTY()
	int32 Priority = 0;

	UPROPERTY()
	int32 GameId = 0;

	UPROPERTY()
	FString Data = "";

	UPROPERTY()
	bool IsActive;

	UPROPERTY()
	int32 Limit = 0;

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


	FNewsFeedEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "NewsFeedEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"NewsFeedId\": 1,\"NewsFeedTypeId\": 1,\"NewsFeedFormatTypeId\": 1,\"Title\":\"title\",\"CreateDate\":\"createdate\",\"Body\":\"body\",\"Priority\": 1,\"GameId\": 1,\"Data\":\"data\",\"IsActive\": 1,\"Limit\": 1,\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FNewsFeedEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FNewsFeedEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("NewsFeedId: %d"), data[0].NewsFeedId);
		UE_LOG(LogCore, Log, TEXT("NewsFeedTypeId: %d"), data[0].NewsFeedTypeId);
		UE_LOG(LogCore, Log, TEXT("NewsFeedFormatTypeId: %d"), data[0].NewsFeedFormatTypeId);
		UE_LOG(LogCore, Log, TEXT("Title: %s"), *data[0].Title);
		UE_LOG(LogCore, Log, TEXT("CreateDate: %s"), *data[0].CreateDate);
		UE_LOG(LogCore, Log, TEXT("Body: %s"), *data[0].Body);
		UE_LOG(LogCore, Log, TEXT("Priority: %d"), data[0].Priority);
		UE_LOG(LogCore, Log, TEXT("GameId: %d"), data[0].GameId);
		UE_LOG(LogCore, Log, TEXT("Data: %s"), *data[0].Data);
		UE_LOG(LogCore, Log, TEXT("Limit: %d"), data[0].Limit);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
