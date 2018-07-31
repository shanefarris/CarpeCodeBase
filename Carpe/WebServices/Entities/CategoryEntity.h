#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "CategoryEntity.generated.h"

USTRUCT()
struct FCategoryEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString GamerTag = "";

	UPROPERTY()
	FString AccountToken = "";

	UPROPERTY()
	int32 AccountType = 0;

	UPROPERTY()
	int32 GameId = 0;

	UPROPERTY()
	FString Email = "";

	UPROPERTY()
	FString Aux1 = "";

	UPROPERTY()
	FString Aux2 = "";

	UPROPERTY()
	int32 ErrorCode = 0;

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


	FCategoryEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "CategoryEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"GamerTag\":\"gamertag\",\"AccountToken\":\"accounttoken\",\"AccountType\": 1,\"GameId\": 1,\"Email\":\"email\",\"Aux1\":\"aux1\",\"Aux2\":\"aux2\",\"ErrorCode\": 1,\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FCategoryEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FCategoryEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("GamerTag: %s"), *data[0].GamerTag);
		UE_LOG(LogCore, Log, TEXT("AccountToken: %s"), *data[0].AccountToken);
		UE_LOG(LogCore, Log, TEXT("AccountType: %d"), data[0].AccountType);
		UE_LOG(LogCore, Log, TEXT("GameId: %d"), data[0].GameId);
		UE_LOG(LogCore, Log, TEXT("Email: %s"), *data[0].Email);
		UE_LOG(LogCore, Log, TEXT("Aux1: %s"), *data[0].Aux1);
		UE_LOG(LogCore, Log, TEXT("Aux2: %s"), *data[0].Aux2);
		UE_LOG(LogCore, Log, TEXT("ErrorCode: %d"), data[0].ErrorCode);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
