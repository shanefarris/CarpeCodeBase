#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "GameItemAttributeEntity.generated.h"

USTRUCT()
struct FGameItemAttributeEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 GameItemAttributeId = 0;

	UPROPERTY()
	int32 GameItemAttributeType = 0;

	UPROPERTY()
	int32 GameItemId = 0;

	UPROPERTY()
	FString StringValue = "";

	UPROPERTY()
	double Value = 0.0f;

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


	FGameItemAttributeEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "GameItemAttributeEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"GameItemAttributeId\": 1,\"GameItemAttributeType\": 1,\"GameItemId\": 1,\"StringValue\":\"stringvalue\",\"Value\": 1,\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FGameItemAttributeEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FGameItemAttributeEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("GameItemAttributeId: %d"), data[0].GameItemAttributeId);
		UE_LOG(LogCore, Log, TEXT("GameItemAttributeType: %d"), data[0].GameItemAttributeType);
		UE_LOG(LogCore, Log, TEXT("GameItemId: %d"), data[0].GameItemId);
		UE_LOG(LogCore, Log, TEXT("StringValue: %s"), *data[0].StringValue);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
