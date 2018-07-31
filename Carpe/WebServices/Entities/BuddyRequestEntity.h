#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "BuddyRequestEntity.generated.h"

USTRUCT()
struct FBuddyRequestEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 BuddyRequestId = 0;

	UPROPERTY()
	int32 BuddyRequestStatusId = 0;

	UPROPERTY()
	FString RequesterGamerTag = "";

	UPROPERTY()
	int32 GameId = 0;

	UPROPERTY()
	FString CreateDate = "";

	UPROPERTY()
	int32 RequestedUserId = 0;

	UPROPERTY()
	int32 RequesterUserId = 0;

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


	FBuddyRequestEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "BuddyRequestEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"BuddyRequestId\": 1,\"BuddyRequestStatusId\": 1,\"RequesterGamerTag\":\"requestergamertag\",\"GameId\": 1,\"CreateDate\":\"createdate\",\"RequestedUserId\": 1,\"RequesterUserId\": 1,\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FBuddyRequestEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FBuddyRequestEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("BuddyRequestId: %d"), data[0].BuddyRequestId);
		UE_LOG(LogCore, Log, TEXT("BuddyRequestStatusId: %d"), data[0].BuddyRequestStatusId);
		UE_LOG(LogCore, Log, TEXT("RequesterGamerTag: %s"), *data[0].RequesterGamerTag);
		UE_LOG(LogCore, Log, TEXT("GameId: %d"), data[0].GameId);
		UE_LOG(LogCore, Log, TEXT("CreateDate: %s"), *data[0].CreateDate);
		UE_LOG(LogCore, Log, TEXT("RequestedUserId: %d"), data[0].RequestedUserId);
		UE_LOG(LogCore, Log, TEXT("RequesterUserId: %d"), data[0].RequesterUserId);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
