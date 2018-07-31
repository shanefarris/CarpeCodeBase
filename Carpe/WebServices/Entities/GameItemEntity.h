#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "GameItemEntity.generated.h"

USTRUCT()
struct FGameItemEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 GameItemId = 0;

	UPROPERTY()
	int32 GameItemTypeId = 0;

	UPROPERTY()
	int32 GameId = 0;

	UPROPERTY()
	FString Title = "";

	UPROPERTY()
	FString Description = "";

	UPROPERTY()
	int32 GasCost = 0;

	UPROPERTY()
	int32 JouleCost = 0;

	UPROPERTY()
	FString LabelName = "";

	UPROPERTY()
	double SalesPercent = 0.0f;

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


	FGameItemEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "GameItemEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"GameItemId\": 1,\"GameItemTypeId\": 1,\"GameId\": 1,\"Title\":\"title\",\"Description\":\"description\",\"GasCost\": 1,\"JouleCost\": 1,\"LabelName\":\"labelname\",\"SalesPercent\": 1,\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FGameItemEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FGameItemEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("GameItemId: %d"), data[0].GameItemId);
		UE_LOG(LogCore, Log, TEXT("GameItemTypeId: %d"), data[0].GameItemTypeId);
		UE_LOG(LogCore, Log, TEXT("GameId: %d"), data[0].GameId);
		UE_LOG(LogCore, Log, TEXT("Title: %s"), *data[0].Title);
		UE_LOG(LogCore, Log, TEXT("Description: %s"), *data[0].Description);
		UE_LOG(LogCore, Log, TEXT("GasCost: %d"), data[0].GasCost);
		UE_LOG(LogCore, Log, TEXT("JouleCost: %d"), data[0].JouleCost);
		UE_LOG(LogCore, Log, TEXT("LabelName: %s"), *data[0].LabelName);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
