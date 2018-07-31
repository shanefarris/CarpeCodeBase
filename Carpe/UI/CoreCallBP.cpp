#include "CoreCallBP.h"
#include "Carpe.h"

UCoreCallBP::UCoreCallBP(const class FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCoreCallBP::CallActorFunc(UUserWidget* UserWidget, FString funcName)
{
	FOutputDeviceNull ar;
	UE_LOG(LogCore, Log, TEXT("%s"), *funcName);
	UserWidget->CallFunctionByNameWithArguments(*funcName, ar, nullptr, true);
}

void UCoreCallBP::CallActorFuncIntParam(UUserWidget* UserWidget, FString funcName, int32 f)
{
	funcName += " " + FString::FromInt(f);
	FOutputDeviceNull ar;
	UserWidget->CallFunctionByNameWithArguments(*funcName, ar, nullptr, true);
}

void UCoreCallBP::CallActorFuncFloatParam(UUserWidget* UserWidget, FString funcName, float f)
{
	funcName += " " + FString::SanitizeFloat(f);
	FOutputDeviceNull ar;
	UserWidget->CallFunctionByNameWithArguments(*funcName, ar, nullptr, true);
}

void UCoreCallBP::CallActorFuncStringParam(UUserWidget* UserWidget, FString funcName, FString s)
{
	funcName += " " + s;
	FOutputDeviceNull ar;
	UserWidget->CallFunctionByNameWithArguments(*funcName, ar, nullptr, true);
}

void UCoreCallBP::CallActorFuncFloatIntParam(UUserWidget* UserWidget, FString funcName, float f, int32 n)
{
	funcName += " " + FString::SanitizeFloat(f) + " " + FString::FromInt(n);
	FOutputDeviceNull ar;
	UserWidget->CallFunctionByNameWithArguments(*funcName, ar, nullptr, true);
}