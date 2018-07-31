#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "BuddyListEntity.generated.h"

USTRUCT()
struct FBuddyListEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 BuddyListId = 0;

	UPROPERTY()
	int32 SmallId = 0;

	UPROPERTY()
	int32 BigId = 0;

	UPROPERTY()
	FString BuddyGamerTag = "";

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


	FBuddyListEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "BuddyListEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"BuddyListId\": 1,\"SmallId\": 1,\"BigId\": 1,\"BuddyGamerTag\":\"buddygamertag\",\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FBuddyListEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FBuddyListEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("BuddyListId: %d"), data[0].BuddyListId);
		UE_LOG(LogCore, Log, TEXT("SmallId: %d"), data[0].SmallId);
		UE_LOG(LogCore, Log, TEXT("BigId: %d"), data[0].BigId);
		UE_LOG(LogCore, Log, TEXT("BuddyGamerTag: %s"), *data[0].BuddyGamerTag);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
