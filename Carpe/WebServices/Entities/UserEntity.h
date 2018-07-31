#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "UserEntity.generated.h"

USTRUCT(BlueprintType)
struct FUserEntity
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "CFG")
	FString LastLoggedIn = "";

	UPROPERTY(BlueprintReadWrite, Category = "CFG")
	FString GamerTag = "";

	UPROPERTY(BlueprintReadWrite, Category = "CFG")
	FString ImageUrl = "";

	UPROPERTY(BlueprintReadWrite, Category = "CFG")
	int32 MethodType = 0;

	UPROPERTY(BlueprintReadWrite, Category = "CFG")
	FString Catalog = "";

	UPROPERTY(BlueprintReadWrite, Category = "CFG")
	int32 UserId = 0;

	UPROPERTY(BlueprintReadWrite, Category = "CFG")
	FString DeviceSerial = "";

	UPROPERTY(BlueprintReadWrite, Category = "CFG")
	int32 Version = 0;

	UPROPERTY(BlueprintReadWrite, Category = "CFG")
	FString EntityName = "";


	FUserEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "UserEntity";
	}

	void GetJsonString(FString& JsonString)
	{
		FJsonObjectConverter::UStructToJsonObjectString(FUserEntity::StaticStruct(), this, JsonString, 0, 0);
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"LastLoggedIn\":\"lastloggedin\",\"GamerTag\":\"gamertag\",\"ImageUrl\":\"imageurl\",\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FUserEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FUserEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("LastLoggedIn: %s"), *data[0].LastLoggedIn);
		UE_LOG(LogCore, Log, TEXT("GamerTag: %s"), *data[0].GamerTag);
		UE_LOG(LogCore, Log, TEXT("ImageUrl: %s"), *data[0].ImageUrl);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
