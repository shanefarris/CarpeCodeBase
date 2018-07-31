#pragma once


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#endif

#include "SaleOfferEntity.generated.h"

USTRUCT()
struct FSaleOfferEntity
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 SaleOfferId = 0;

	UPROPERTY()
	int32 Offer = 0;

	UPROPERTY()
	int32 OfferType = 0;

	UPROPERTY()
	int32 GameId = 0;

	UPROPERTY()
	int32 GameItemId = 0;

	UPROPERTY()
	FString Expire = "";

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


	FSaleOfferEntity()
	{
		MethodType = 0;
		Catalog = "";
		UserId = 0;
		DeviceSerial = "";
		Version = 1;
		EntityName = "SaleOfferEntity";
	}


#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	static FString GetJsonExample()
	{
		return "[{\"SaleOfferId\": 1,\"Offer\": 1,\"OfferType\": 1,\"GameId\": 1,\"GameItemId\": 1,\"Expire\":\"expire\",\"MethodType\": 1,\"Catalog\":\"catalog\",\"UserId\": 1,\"DeviceSerial\":\"deviceserial\",\"Version\": 1,\"EntityName\":\"entityname\"}]";
	}

	static void Check()
	{
		TArray<FSaleOfferEntity> data;
		FJsonObjectConverter::JsonArrayStringToUStruct(FSaleOfferEntity::GetJsonExample(), &data, 0, 0);
		UE_LOG(LogCore, Log, TEXT("SaleOfferId: %d"), data[0].SaleOfferId);
		UE_LOG(LogCore, Log, TEXT("Offer: %d"), data[0].Offer);
		UE_LOG(LogCore, Log, TEXT("OfferType: %d"), data[0].OfferType);
		UE_LOG(LogCore, Log, TEXT("GameId: %d"), data[0].GameId);
		UE_LOG(LogCore, Log, TEXT("GameItemId: %d"), data[0].GameItemId);
		UE_LOG(LogCore, Log, TEXT("Expire: %s"), *data[0].Expire);
		UE_LOG(LogCore, Log, TEXT("MethodType: %d"), data[0].MethodType);
		UE_LOG(LogCore, Log, TEXT("Catalog: %s"), *data[0].Catalog);
		UE_LOG(LogCore, Log, TEXT("UserId: %d"), data[0].UserId);
		UE_LOG(LogCore, Log, TEXT("DeviceSerial: %s"), *data[0].DeviceSerial);
		UE_LOG(LogCore, Log, TEXT("Version: %d"), data[0].Version);
		UE_LOG(LogCore, Log, TEXT("EntityName: %s"), *data[0].EntityName);
	}

#endif
};
