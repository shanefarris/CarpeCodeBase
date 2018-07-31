#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "IapPackageEntity.generated.h"

USTRUCT()
struct FIapPackageEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 IapPackageId = 0;

	UPROPERTY()
	int32 GameId = 0;

	UPROPERTY()
	FString Description = "";

	UPROPERTY()
	FString ProductLabel = "";

	UPROPERTY()
	int32 QtyGas = 0;

	UPROPERTY()
	int32 GamePlatformTypeId = 0;

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


	FIapPackageEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "IapPackageEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"IapPackageId\": 1,\"GameId\": 1,\"Description\":\"description\",\"ProductLabel\":\"productlabel\",\"QtyGas\": 1,\"GamePlatformTypeId\": 1,\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FIapPackageEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FIapPackageEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("IapPackageId: %d"), data[0].IapPackageId);
		UE_LOG(LogCore, Log, TEXT("GameId: %d"), data[0].GameId);
		UE_LOG(LogCore, Log, TEXT("Description: %s"), *data[0].Description);
		UE_LOG(LogCore, Log, TEXT("ProductLabel: %s"), *data[0].ProductLabel);
		UE_LOG(LogCore, Log, TEXT("QtyGas: %d"), data[0].QtyGas);
		UE_LOG(LogCore, Log, TEXT("GamePlatformTypeId: %d"), data[0].GamePlatformTypeId);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
