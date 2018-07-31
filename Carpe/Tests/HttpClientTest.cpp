#include "Carpe.h"
#include "Webservices/HttpClient.h"
#include "AutomationTest.h"

void Ping_Callback(const FString& Json)
{
}

//IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPingTest, "Ping", EAutomationTestFlags::EditorContext)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPingTest, "Ping", EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter)
bool FPingTest::RunTest(const FString& Parameters)
{
	auto httpClient = NewObject<UHttpClient>();
	httpClient->Ping_Callback.BindStatic(&Ping_Callback);
	httpClient->Ping();

	return true;
}

void GetUser_Callback(const FUserEntity&)
{
}

//IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPingTest, "Ping", EAutomationTestFlags::EditorContext)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetUser, "GetUser", EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter)
bool FGetUser::RunTest(const FString& Parameters)
{
	auto httpClient = NewObject<UHttpClient>();
	httpClient->GetUser_Callback.BindStatic(&GetUser_Callback);
	httpClient->GetUser(2);

	return true;
}