#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "MessageEntity.generated.h"

USTRUCT()
struct FMessageEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Subject = "";

	UPROPERTY()
	FString Message = "";

	UPROPERTY()
	int32 MessageId = 0;

	UPROPERTY()
	int32 MessageType = 0;

	UPROPERTY()
	int32 MessageStatus = 0;

	UPROPERTY()
	int32 To = 0;

	UPROPERTY()
	int32 From = 0;

	UPROPERTY()
	FString FromGamerTag = "";

	UPROPERTY()
	FString Date = "";

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


	FMessageEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "MessageEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"Subject\":\"subject\",\"Message\":\"message\",\"MessageId\": 1,\"MessageType\": 1,\"MessageStatus\": 1,\"To\": 1,\"From\": 1,\"FromGamerTag\":\"fromgamertag\",\"Date\":\"date\",\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FMessageEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FMessageEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("Subject: %s"), *data[0].Subject);
		UE_LOG(LogCore, Log, TEXT("Message: %s"), *data[0].Message);
		UE_LOG(LogCore, Log, TEXT("MessageId: %d"), data[0].MessageId);
		UE_LOG(LogCore, Log, TEXT("MessageType: %d"), data[0].MessageType);
		UE_LOG(LogCore, Log, TEXT("MessageStatus: %d"), data[0].MessageStatus);
		UE_LOG(LogCore, Log, TEXT("To: %d"), data[0].To);
		UE_LOG(LogCore, Log, TEXT("From: %d"), data[0].From);
		UE_LOG(LogCore, Log, TEXT("FromGamerTag: %s"), *data[0].FromGamerTag);
		UE_LOG(LogCore, Log, TEXT("Date: %s"), *data[0].Date);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
