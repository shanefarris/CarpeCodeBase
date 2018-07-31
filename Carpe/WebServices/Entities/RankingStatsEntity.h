#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "RankingStatsEntity.generated.h"

USTRUCT()
struct FRankingStatsEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 Count = 0;

	UPROPERTY()
	int32 RankingType = 0;

	UPROPERTY()
	int32 RankingSearchType = 0;

	UPROPERTY()
	int32 Aux1 = 0;

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


	FRankingStatsEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "RankingStatsEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"Entries\": 1,\"Count\": 1,\"RankingType\": 1,\"RankingSearchType\": 1,\"Aux1\": 1,\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FRankingStatsEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FRankingStatsEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("Count: %d"), data[0].Count);
		UE_LOG(LogCore, Log, TEXT("RankingType: %d"), data[0].RankingType);
		UE_LOG(LogCore, Log, TEXT("RankingSearchType: %d"), data[0].RankingSearchType);
		UE_LOG(LogCore, Log, TEXT("Aux1: %d"), data[0].Aux1);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
