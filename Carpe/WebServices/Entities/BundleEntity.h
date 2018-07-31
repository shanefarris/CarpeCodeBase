#pragma once

#include "BundleItemEntity.h"

#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "BundleEntity.generated.h"

USTRUCT()
struct FBundleEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 BundleId = 0;

	UPROPERTY()
	FString Title = "";

	UPROPERTY()
	FString Description = "";

	UPROPERTY()
	FString LabelName = "";

	UPROPERTY()
	int32 GameId = 0;

	UPROPERTY()
	int32 Gas = 0;

	UPROPERTY()
	int32 Joules = 0;

	UPROPERTY()
	int32 Order = 0;

	UPROPERTY()
	FString ImageUrl = "";

	UPROPERTY()
	FString StartDate = "";

	UPROPERTY()
	FString EndDate = "";

	UPROPERTY()
	TArray<FBundleItemEntity> _bundleItems;

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


	FBundleEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "BundleEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"BundleId\": 1,\"Title\":\"title\",\"Description\":\"description\",\"LabelName\":\"labelname\",\"GameId\": 1,\"Gas\": 1,\"Joules\": 1,\"Order\": 1,\"ImageUrl\":\"imageurl\",\"StartDate\":\"startdate\",\"EndDate\":\"enddate\",\"_bundleItems\": 1,\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FBundleEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FBundleEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("BundleId: %d"), data[0].BundleId);
		UE_LOG(LogCore, Log, TEXT("Title: %s"), *data[0].Title);
		UE_LOG(LogCore, Log, TEXT("Description: %s"), *data[0].Description);
		UE_LOG(LogCore, Log, TEXT("LabelName: %s"), *data[0].LabelName);
		UE_LOG(LogCore, Log, TEXT("GameId: %d"), data[0].GameId);
		UE_LOG(LogCore, Log, TEXT("Gas: %d"), data[0].Gas);
		UE_LOG(LogCore, Log, TEXT("Joules: %d"), data[0].Joules);
		UE_LOG(LogCore, Log, TEXT("ImageUrl: %s"), *data[0].ImageUrl);
		UE_LOG(LogCore, Log, TEXT("StartDate: %s"), *data[0].StartDate);
		UE_LOG(LogCore, Log, TEXT("EndDate: %s"), *data[0].EndDate);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
