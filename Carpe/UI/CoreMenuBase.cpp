#include "CoreMenuBase.h"
#include "Carpe.h"
#include "WebServices/HttpClient.h"
#include "WebServices/ErrorMessage.h"
#include "CoreCallBP.h"

#include "Kismet/BlueprintFunctionLibrary.h"

bool UCoreMenuBase::CreateNewUser(const FString& UserName, const FString Password)
{
	//auto httpClient = (UHttpClient*)UHttpClient::StaticClass()->GetDefaultObject();
	
	// Create user and account
	//httpClient->CreateNewUser_Callback.BindUObject(this, &UCoreMenuBase::CreateNewUser_Callback);
	//httpClient->CreateNewUser(Password, UserName);
	
	return true;
}

void UCoreMenuBase::Ping()
{
	//auto httpClient = (UHttpClient*)UHttpClient::StaticClass()->GetDefaultObject();
	//httpClient->Ping();
}

bool UCoreMenuBase::Login(const FString& UserName, const FString Password)
{
	if (UserName.Len() > 0 && Password.Len() > 0)
		return true;

	return false;
}

void UCoreMenuBase::CreateNewUser_Callback(int32 UserId)
{
	FString Report;

	if (UserId > 0)
	{
		Report = "User successfully created";
	}
	else if (UserId == ErrorCodes::UniqueName)
	{
		Report = "User name already in use.";
	}
	else if (UserId == ErrorCodes::GamerTagInvalid)
	{
		Report = "User name has invalid characters.";
	}
	else if (UserId == ErrorCodes::WeakPassword)
	{
		Report = "Weak password.";
	}
	else
	{
		Report = "Error creating user.";
	}

	UCoreCallBP::CallActorFuncStringParam(this, "ReportUserCreation", Report);
}

bool UCoreMenuBase::CreateNewAccount(int32 UserId, const FString& UserName, const FString& Password, const FString& Email)
{
	//auto httpClient = (UHttpClient*)UHttpClient::StaticClass()->GetDefaultObject();

	// Create user and account
	//httpClient->CreateNewAccount_Callback.BindUObject(this, &UCoreMenuBase::CreateNewAccount_Callback);
	//httpClient->CreateNewAccount(UserId, UserName, Password, Email);

	return true;
}

void UCoreMenuBase::CreateNewAccount_Callback(const FCategoryEntity& Entity)
{
}