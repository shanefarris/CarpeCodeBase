#include "HttpClient.h"
#include "Carpe.h"
#include "Runtime/JsonUtilities/Public/JsonUtilities.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"

#include "Entities/ErrorEntity.h"
#include "Entities/UserEntity.h"
#include "Entities/CategoryEntity.h"

#define METHOD_TYPE_NONE 0
#define METHOD_TYPE_CREATE 1
#define METHOD_TYPE_UPDATE 2
#define METHOD_TYPE_DELETE 3
#define METHOD_TYPE_GET 4
#define CLIENT_UNKNOWN_ERROR "-988"

UHttpClient::UHttpClient(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsBusy = false;
	TargetHost = FString("http://api.cornfedgames.com/api/");
	//TargetHost = FString("http://localhost:2000/api/");
	UE_LOG(LogCore, Log, TEXT("%s"), *TargetHost);
}

TSharedRef<IHttpRequest> UHttpClient::SetupTransmission()
{
	bIsBusy = true;
	CurrentRequest = RequestQueue[0];
	RequestQueue.RemoveAt(0);

	auto Request = FHttpModule::Get().CreateRequest();
	if (!FHttpModule::Get().IsHttpEnabled()) 
		return Request;
	
	Request->SetVerb(CurrentRequest.Method);
	Request->SetURL(TargetHost + CurrentRequest.Controller);
	if (CurrentRequest.Parameters.Len() > 0)
	{
		if (CurrentRequest.Method == "POST")
		{
			Request->SetContentAsString("=" + CurrentRequest.Parameters);
		}
		UE_LOG(LogCore, Log, TEXT("Sending JSON: %s"), *CurrentRequest.Parameters);
	}
	Request->SetHeader("User-Agent", "Core/1.0");
	Request->SetHeader("Content-Type", "application/x-www-form-urlencoded");

	return Request;
}

void UHttpClient::FinishRequest()
{
	// Retry transmission in case there are any queued messages (if not, nothing happens)
	if (RequestQueue.Num() > 0)
	{
		SetupTransmission();
	}
	else
	{
		bIsBusy = false;
	}
}

void UHttpClient::Ping()
{
	FHttpSetupRequest SetupRequest;
	FString Controller("ping");

	SetupRequest.Controller = Controller;
	SetupRequest.Method = "GET";
	SetupRequest.HttpRequestCompleteDelegate.BindUObject(this, &UHttpClient::OnResponseReceived_Ping);

	RequestQueue.Add(SetupRequest);

	if (!bIsBusy)
	{
		auto Request = SetupTransmission();

		Request->OnProcessRequestComplete().BindUObject(this, &UHttpClient::OnResponseReceived_Ping);
		if (!Request->ProcessRequest())
		{
			UE_LOG(LogCore, Log, TEXT("Unable to process HTTP Request!"));
			bIsBusy = false;
		}
	}
}

void UHttpClient::OnResponseReceived_Ping(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString Json = "";

	// If HTTP fails client-side, this will still be called but with a NULL shared pointer!
	if (!Response.IsValid())
	{
		Json = CLIENT_UNKNOWN_ERROR;
	}
	else
	{
		Json = Response->GetContentAsString();
	}

	Ping_Callback.ExecuteIfBound(Json);
	UE_LOG(LogCore, Log, TEXT("%s"), *Json);

	FinishRequest();
}

void UHttpClient::CreateNewUser(const FString& Password, const FString& GamerTag)
{
	FUserEntity entity;
	entity.Catalog = Password;
	entity.GamerTag = GamerTag;
	entity.MethodType = METHOD_TYPE_CREATE;

	FString json;
	FJsonObjectConverter::UStructToJsonObjectString(FUserEntity::StaticStruct(), &entity, json, 0, 0);

	FHttpSetupRequest SetupRequest;
	SetupRequest.Controller = "user";
	SetupRequest.Parameters = json;
	SetupRequest.Method = "POST";
	SetupRequest.HttpRequestCompleteDelegate.BindUObject(this, &UHttpClient::OnResponseReceived_CreateNewUser);

	RequestQueue.Add(SetupRequest);

	if (!bIsBusy)
	{
		auto Request = SetupTransmission();

		Request->OnProcessRequestComplete().BindUObject(this, &UHttpClient::OnResponseReceived_CreateNewUser);
		if (!Request->ProcessRequest())
		{
			UE_LOG(LogCore, Log, TEXT("Unable to process HTTP Request!"));
			bIsBusy = false;
		}
	}
}

void UHttpClient::OnResponseReceived_CreateNewUser(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString Json = "";

	// If HTTP fails client-side, this will still be called but with a NULL shared pointer!
	if (!Response.IsValid())
	{
		Json = CLIENT_UNKNOWN_ERROR;
	}
	else
	{
		Json = Response->GetContentAsString();
		if (Json.IsNumeric())
		{
			auto id = FCString::Atoi(*Json);
			if (id > 0)
			{
				CreateNewUser_Callback.ExecuteIfBound(id);
			}
			else
			{
				UE_LOG(LogCore, Log, TEXT("Error creating new user, error: %s"), *Json);
				CreateNewUser_Callback.ExecuteIfBound(id);
			}
		}
		else
		{
			UE_LOG(LogCore, Log, TEXT("Error creating new user"));
			CreateNewUser_Callback.ExecuteIfBound(-999);
		}
	}

	FinishRequest();
}

void UHttpClient::GetUser(int32 InUserId)
{
	FHttpSetupRequest SetupRequest;
	FString Controller("user");

	FUserEntity entity;
	entity.UserId = InUserId;
	entity.GamerTag = "REQUEST";

	entity.MethodType = METHOD_TYPE_GET;

	FString json;
	FJsonObjectConverter::UStructToJsonObjectString(FUserEntity::StaticStruct(), &entity, json, 0, 0, 0);

	SetupRequest.Controller = Controller + "/" + json;
	SetupRequest.Parameters = json;
	SetupRequest.Method = "GET";
	SetupRequest.HttpRequestCompleteDelegate.BindUObject(this, &UHttpClient::OnResponseReceived_GetUser);

	RequestQueue.Add(SetupRequest);

	if (!bIsBusy)
	{
		auto Request = SetupTransmission();

		Request->OnProcessRequestComplete().BindUObject(this, &UHttpClient::OnResponseReceived_GetUser);
		if (!Request->ProcessRequest())
		{
			UE_LOG(LogCore, Log, TEXT("Unable to process HTTP Request!"));
		}
	}
}

void UHttpClient::OnResponseReceived_GetUser(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString Json = "";

	if (!Response.IsValid())
	{
		Json = CLIENT_UNKNOWN_ERROR;
	}
	else
	{
		Json = Response->GetContentAsString();
	}

	FUserEntity entity;
	FJsonObjectConverter::JsonObjectStringToUStruct<FUserEntity>(Json, &entity, 0, 0);

	GetUser_Callback.ExecuteIfBound(entity);
	UE_LOG(LogCore, Log, TEXT("%s"), *Json);

	FinishRequest();
}

void UHttpClient::CreateNewAccount(int32 UserId, const FString& GamerTag, const FString& Password, const FString& Email)
{
	FCategoryEntity entity;
	entity.Catalog = Password;
	entity.GamerTag = GamerTag;
	entity.Email = Email;
	entity.UserId = UserId;
	entity.MethodType = METHOD_TYPE_CREATE;

	FString json;
	FJsonObjectConverter::UStructToJsonObjectString(FUserEntity::StaticStruct(), &entity, json, 0, 0);

	FHttpSetupRequest SetupRequest;
	SetupRequest.Controller = "category";
	SetupRequest.Parameters = json;
	SetupRequest.Method = "POST";
	SetupRequest.HttpRequestCompleteDelegate.BindUObject(this, &UHttpClient::OnResponseReceived_CreateNewAccount);

	RequestQueue.Add(SetupRequest);

	if (!bIsBusy)
	{
		auto Request = SetupTransmission();

		Request->OnProcessRequestComplete().BindUObject(this, &UHttpClient::OnResponseReceived_CreateNewAccount);
		if (!Request->ProcessRequest())
		{
			UE_LOG(LogCore, Log, TEXT("Unable to process HTTP Request!"));
			bIsBusy = false;
		}
	}
}

void UHttpClient::OnResponseReceived_CreateNewAccount(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString Json = "";

	if (!Response.IsValid())
	{
		Json = CLIENT_UNKNOWN_ERROR;
	}
	else
	{
		Json = Response->GetContentAsString();
	}

	FCategoryEntity entity;
	FJsonObjectConverter::JsonObjectStringToUStruct<FCategoryEntity>(Json, &entity, 0, 0);

	CreateNewAccount_Callback.ExecuteIfBound(entity);
	UE_LOG(LogCore, Log, TEXT("%s"), *Json);

	FinishRequest();
}