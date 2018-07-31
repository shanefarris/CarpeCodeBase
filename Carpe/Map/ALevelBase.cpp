#include "ALevelBase.h"
#include "Carpe.h"

//#include "ThirdParty/Steamworks/Steamv132/sdk/public/steam/steam_api.h"

ALevelBase::ALevelBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void ALevelBase::BeginPlay()
{
	Super::ReceiveBeginPlay();
	UE_LOG(LogCore,Error,TEXT("STREAMING LEVEL BEGIN PLAY %s"), *GetName());

	//if (SteamAPI_Init())
	//{
	//	//do my steam stuff
	//}
	//else
	//{
	//	//push an error or something
	//}
}

void ALevelBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UE_LOG(LogCore,Error,TEXT("STREAMING LEVEL UNSTREAMED (name should not be none) %s"), *GetName());
	UE_LOG(LogCore,Error,TEXT("Victory!!!"));
	UE_LOG(LogCore,Error,TEXT("RUN ALL UNSTREAMING LEVEL LOGIC HERE"));

	//Save Level Just Before Exiting
	UpdateLevelSaveData();

	Super::EndPlay(EndPlayReason);
}

void ALevelBase::UpdateLevelSaveData()
{
	//Save Level Data
}

void ALevelBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}