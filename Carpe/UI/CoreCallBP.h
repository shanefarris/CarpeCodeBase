#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreCallBP.generated.h"

UCLASS()
class UCoreCallBP : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = CoreCallBP)
	static void CallActorFunc(UUserWidget* UserWidget, FString funcName);

	UFUNCTION(BlueprintCallable, Category = CoreCallBP)
	static void CallActorFuncIntParam(UUserWidget* UserWidget, FString funcName, int32 f);

	UFUNCTION(BlueprintCallable, Category = CoreCallBP)
	static void CallActorFuncFloatParam(UUserWidget* UserWidget, FString funcName, float f);

	UFUNCTION(BlueprintCallable, Category = CoreCallBP)
	static void CallActorFuncStringParam(UUserWidget* UserWidget, FString funcName, FString s);

	UFUNCTION(BlueprintCallable, Category = CoreCallBP)
	static void CallActorFuncFloatIntParam(UUserWidget* UserWidget, FString funcName, float f, int32 n);
};