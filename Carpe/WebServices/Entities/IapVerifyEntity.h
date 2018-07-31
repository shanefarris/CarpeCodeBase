#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "IapVerifyEntity.generated.h"

USTRUCT()
struct FIapVerifyEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 GamePlatformType = 0;

	UPROPERTY()
	FString Token1 = "";

	UPROPERTY()
	FString Token2 = "";

	UPROPERTY()
	FString ProductName = "";

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


	FIapVerifyEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "IapVerifyEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"GamePlatformType\": 1,\"Token1\":\"token1\",\"Token2\":\"token2\",\"ProductName\":\"productname\",\"GameId\": 1,\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FIapVerifyEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FIapVerifyEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("GamePlatformType: %d"), data[0].GamePlatformType);
		UE_LOG(LogCore, Log, TEXT("Token1: %s"), *data[0].Token1);
		UE_LOG(LogCore, Log, TEXT("Token2: %s"), *data[0].Token2);
		UE_LOG(LogCore, Log, TEXT("ProductName: %s"), *data[0].ProductName);
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
