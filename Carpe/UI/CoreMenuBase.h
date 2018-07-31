// Copyright 2015 Corn Fed Games LLC

#pragma once

#include "Blueprint/UserWidget.h"
#include "Templates/SharedPointer.h"
#include "CoreMenuBase.generated.h"

struct FCategoryEntity;

UCLASS()
class UCoreMenuBase : public UUserWidget
{
	GENERATED_BODY()

public:
	void CreateNewUser_Callback(int32 UserId);
	void CreateNewAccount_Callback(const FCategoryEntity& Entity);

	UFUNCTION(BlueprintCallable, BlueprintCallable, meta = (DisplayName = "Ping"), Category = "Menu")
	void Ping();

	UFUNCTION(BlueprintCallable, BlueprintCallable, meta = (DisplayName = "Create New User"), Category = "Menu")
	bool CreateNewUser(const FString& UserName, const FString Password);

	UFUNCTION(BlueprintCallable, BlueprintCallable, meta = (DisplayName = "Create New Account"), Category = "Menu")
	bool CreateNewAccount(int32 UserId, const FString& UserName, const FString& Password, const FString& Email);

	UFUNCTION(BlueprintCallable, BlueprintCallable, meta = (DisplayName = "Login"), Category = "Menu")
	bool Login(const FString& UserName, const FString Password);


};

