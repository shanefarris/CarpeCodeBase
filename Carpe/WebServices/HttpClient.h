
#pragma once

#include "Http.h"
#include "Containers/Array.h"
#include "HttpClient.generated.h"

USTRUCT()
struct FHttpSetupRequest
{
	GENERATED_USTRUCT_BODY()

	FString Controller;
	FString Parameters;
	FString Method;
	FHttpRequestCompleteDelegate HttpRequestCompleteDelegate;

	FHttpSetupRequest() : Controller(""), Parameters(""), Method("GET") { }
};

struct FUserEntity;
struct FCategoryEntity;

DECLARE_DELEGATE_OneParam(FPingDelegate, const FString&);
DECLARE_DELEGATE_OneParam(FCreateNewUserDelegate, int32);
DECLARE_DELEGATE_OneParam(FGetUserDelegate, const FUserEntity&);
DECLARE_DELEGATE_OneParam(FCreateNewAccountDelegate, const FCategoryEntity&);

UCLASS(Abstract, Blueprintable)
class UHttpClient : public UObject
{
	GENERATED_UCLASS_BODY()

private:
	FHttpModule* Http;
	FString TargetHost;
	bool bIsBusy;

	TArray<FHttpSetupRequest> RequestQueue;
	FHttpSetupRequest CurrentRequest;

	TSharedRef<IHttpRequest> SetupTransmission();

	void OnResponseReceived_Ping(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful); 
	void OnResponseReceived_CreateNewUser(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnResponseReceived_CreateNewAccount(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnResponseReceived_GetUser(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void FinishRequest();

public:
	FPingDelegate				Ping_Callback;
	FCreateNewUserDelegate		CreateNewUser_Callback;
	FGetUserDelegate			GetUser_Callback;
	FCreateNewAccountDelegate	CreateNewAccount_Callback;

	// Ping
	UFUNCTION(BlueprintCallable, Category = "CFG")
	void Ping();

	// User
	void CreateNewUser(const FString& Password, const FString& GamerTag);
	void GetUser(int32 UserId);
	
	// Account
	void CreateNewAccount(int32 UserId, const FString& UserName, const FString& Password, const FString& Email);
};