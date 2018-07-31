#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "UserLeaderBoardEntity.generated.h"

USTRUCT()
struct FUserLeaderBoardEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 Score = 0;

	UPROPERTY()
	int32 GameId = 0;

	UPROPERTY()
	int32 LeaderBoardId = 0;

	UPROPERTY()
	FString GamerTag = "";

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


	FUserLeaderBoardEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "UserLeaderBoardEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"Score\": 1,\"GameId\": 1,\"LeaderBoardId\": 1,\"GamerTag\":\"gamertag\",\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FUserLeaderBoardEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FUserLeaderBoardEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("Score: %d"), data[0].Score);
		UE_LOG(LogCore, Log, TEXT("GameId: %d"), data[0].GameId);
		UE_LOG(LogCore, Log, TEXT("LeaderBoardId: %d"), data[0].LeaderBoardId);
		UE_LOG(LogCore, Log, TEXT("GamerTag: %s"), *data[0].GamerTag);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
