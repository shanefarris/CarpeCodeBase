#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "UserEconomyEntity.generated.h"

USTRUCT()
struct FUserEconomyEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 UserEconomyId = 0;

	UPROPERTY()
	int32 GameId = 0;

	UPROPERTY()
	int32 Joules = 0;

	UPROPERTY()
	int32 Gas = 0;

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


	FUserEconomyEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "UserEconomyEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"UserEconomyId\": 1,\"GameId\": 1,\"Joules\": 1,\"Gas\": 1,\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FUserEconomyEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FUserEconomyEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("UserEconomyId: %d"), data[0].UserEconomyId);
		UE_LOG(LogCore, Log, TEXT("GameId: %d"), data[0].GameId);
		UE_LOG(LogCore, Log, TEXT("Joules: %d"), data[0].Joules);
		UE_LOG(LogCore, Log, TEXT("Gas: %d"), data[0].Gas);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
