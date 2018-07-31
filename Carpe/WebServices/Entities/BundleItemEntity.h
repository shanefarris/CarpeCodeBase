#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "BundleItemEntity.generated.h"

USTRUCT()
struct FBundleItemEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 BundleId = 0;

	UPROPERTY()
	int32 BundleItemId = 0;

	UPROPERTY()
	int32 GameItemId = 0;

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


	FBundleItemEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "BundleItemEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"BundleId\": 1,\"BundleItemId\": 1,\"GameItemId\": 1,\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FBundleItemEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FBundleItemEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("BundleId: %d"), data[0].BundleId);
		UE_LOG(LogCore, Log, TEXT("BundleItemId: %d"), data[0].BundleItemId);
		UE_LOG(LogCore, Log, TEXT("GameItemId: %d"), data[0].GameItemId);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
