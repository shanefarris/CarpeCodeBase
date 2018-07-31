#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "LeaderBoardEntity.generated.h"

USTRUCT()
struct FLeaderBoardEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 LeaderBoardId = 0;

	UPROPERTY()
	int32 GameId = 0;

	UPROPERTY()
	int32 GameTypeId = 0;

	UPROPERTY()
	FString Title = "";

	UPROPERTY()
	int32 ScoreMin = 0;

	UPROPERTY()
	int32 ScoreMax = 0;

	UPROPERTY()
	int32 ScoreUnitTypeId = 0;

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


	FLeaderBoardEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "LeaderBoardEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"LeaderBoardId\": 1,\"GameId\": 1,\"GameTypeId\": 1,\"Title\":\"title\",\"ScoreMin\": 1,\"ScoreMax\": 1,\"ScoreUnitTypeId\": 1,\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FLeaderBoardEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FLeaderBoardEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("LeaderBoardId: %d"), data[0].LeaderBoardId);
		UE_LOG(LogCore, Log, TEXT("GameId: %d"), data[0].GameId);
		UE_LOG(LogCore, Log, TEXT("GameTypeId: %d"), data[0].GameTypeId);
		UE_LOG(LogCore, Log, TEXT("Title: %s"), *data[0].Title);
		UE_LOG(LogCore, Log, TEXT("ScoreMin: %d"), data[0].ScoreMin);
		UE_LOG(LogCore, Log, TEXT("ScoreMax: %d"), data[0].ScoreMax);
		UE_LOG(LogCore, Log, TEXT("ScoreUnitTypeId: %d"), data[0].ScoreUnitTypeId);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
