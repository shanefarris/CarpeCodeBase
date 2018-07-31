#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "ClanEntity.generated.h"

USTRUCT()
struct FClanEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 ClanId = 0;

	UPROPERTY()
	int32 GameId = 0;

	UPROPERTY()
	int32 TopLimit = 0;

	UPROPERTY()
	TArray<FString> Members;

	UPROPERTY()
	FString Description = "";

	UPROPERTY()
	FString Name = "";

	UPROPERTY()
	bool IsInviteOnly;

	UPROPERTY()
	bool IsRequest;

	UPROPERTY()
	FString LastActive = "";

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


	FClanEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "ClanEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"ClanId\": 1,\"GameId\": 1,\"TopLimit\": 1,\"Members\": 1,\"Description\":\"description\",\"Name\":\"name\",\"IsInviteOnly\": 1,\"IsRequest\": 1,\"LastActive\":\"lastactive\",\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FClanEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FClanEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("ClanId: %d"), data[0].ClanId);
		UE_LOG(LogCore, Log, TEXT("GameId: %d"), data[0].GameId);
		UE_LOG(LogCore, Log, TEXT("TopLimit: %d"), data[0].TopLimit);
		UE_LOG(LogCore, Log, TEXT("Description: %s"), *data[0].Description);
		UE_LOG(LogCore, Log, TEXT("Name: %s"), *data[0].Name);
		UE_LOG(LogCore, Log, TEXT("LastActive: %s"), *data[0].LastActive);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
