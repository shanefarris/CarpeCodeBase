#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "PatchEntity.generated.h"

USTRUCT()
struct FPatchEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 GameId = 0;

	UPROPERTY()
	int32 GameVersion = 0;

	UPROPERTY()
	FString PatchLocation = "";

	UPROPERTY()
	int32 PatchSize = 0;

	UPROPERTY()
	FString Hash = "";

	UPROPERTY()
	int32 GamePlatformType = 0;

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


	FPatchEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "PatchEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"GameId\": 1,\"GameVersion\": 1,\"PatchLocation\":\"patchlocation\",\"PatchSize\": 1,\"Hash\":\"hash\",\"GamePlatformType\": 1,\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FPatchEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FPatchEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("GameId: %d"), data[0].GameId);
		UE_LOG(LogCore, Log, TEXT("GameVersion: %d"), data[0].GameVersion);
		UE_LOG(LogCore, Log, TEXT("PatchLocation: %s"), *data[0].PatchLocation);
		UE_LOG(LogCore, Log, TEXT("PatchSize: %d"), data[0].PatchSize);
		UE_LOG(LogCore, Log, TEXT("Hash: %s"), *data[0].Hash);
		UE_LOG(LogCore, Log, TEXT("GamePlatformType: %d"), data[0].GamePlatformType);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
