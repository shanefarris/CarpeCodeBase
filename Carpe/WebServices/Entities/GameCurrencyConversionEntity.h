#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "GameCurrencyConversionEntity.generated.h"

USTRUCT()
struct FGameCurrencyConversionEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 GameCurrencyConversionId = 0;

	UPROPERTY()
	int32 GameId = 0;

	UPROPERTY()
	bool IsEnabled;

	UPROPERTY()
	int32 GasConsumed = 0;

	UPROPERTY()
	int32 JoulesRewarded = 0;

	UPROPERTY()
	FString Name = "";

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


	FGameCurrencyConversionEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "GameCurrencyConversionEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"GameCurrencyConversionId\": 1,\"GameId\": 1,\"IsEnabled\": 1,\"GasConsumed\": 1,\"JoulesRewarded\": 1,\"Name\":\"name\",\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FGameCurrencyConversionEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FGameCurrencyConversionEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("GameCurrencyConversionId: %d"), data[0].GameCurrencyConversionId);
		UE_LOG(LogCore, Log, TEXT("GameId: %d"), data[0].GameId);
		UE_LOG(LogCore, Log, TEXT("GasConsumed: %d"), data[0].GasConsumed);
		UE_LOG(LogCore, Log, TEXT("JoulesRewarded: %d"), data[0].JoulesRewarded);
		UE_LOG(LogCore, Log, TEXT("Name: %s"), *data[0].Name);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
