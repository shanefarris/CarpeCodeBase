#include "Player/GameCheatManager.h"
#include "Carpe.h"
#include "Player/GamePlayerState.h"
#include "AI/AIControllerBase.h"

UGameCheatManager::UGameCheatManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGameCheatManager::ToggleInfiniteAmmo()
{
	AGamePlayerController* MyPC = GetOuterAGamePlayerController();

	MyPC->SetInfiniteAmmo(!MyPC->HasInfiniteAmmo());
	MyPC->ClientMessage(FString::Printf(TEXT("Infinite ammo: %s"), MyPC->HasInfiniteAmmo() ? TEXT("ENABLED") : TEXT("off")));
}

void UGameCheatManager::ToggleInfiniteClip()
{
	AGamePlayerController* MyPC = GetOuterAGamePlayerController();

	MyPC->SetInfiniteClip(!MyPC->HasInfiniteClip());
	MyPC->ClientMessage(FString::Printf(TEXT("Infinite clip: %s"), MyPC->HasInfiniteClip() ? TEXT("ENABLED") : TEXT("off")));
}

void UGameCheatManager::ToggleMatchTimer()
{
	AGamePlayerController* MyPC = GetOuterAGamePlayerController();

	ACoreGameState* const MyGameState = Cast<ACoreGameState>(MyPC->GetWorld()->GetGameState());
	if (MyGameState && MyGameState->Role == ROLE_Authority)
	{
		MyGameState->bTimerPaused = !MyGameState->bTimerPaused;
		MyPC->ClientMessage(FString::Printf(TEXT("Match timer: %s"), MyGameState->bTimerPaused ? TEXT("PAUSED") : TEXT("running")));
	}
}

void UGameCheatManager::ForceMatchStart()
{
	AGamePlayerController* const MyPC = GetOuterAGamePlayerController();

	AGameMode* const MyGame = MyPC->GetWorld()->GetAuthGameMode<AGameMode>();
	if (MyGame && MyGame->GetMatchState() == MatchState::WaitingToStart)
	{
		MyGame->StartMatch();
	}
}

void UGameCheatManager::ChangeTeam(int32 NewTeamNumber)
{
	AGamePlayerController* MyPC = GetOuterAGamePlayerController();

	auto MyPlayerState = Cast<AGamePlayerState>(MyPC->PlayerState);
	if (MyPlayerState && MyPlayerState->Role == ROLE_Authority)
	{
		MyPlayerState->SetTeamNum(NewTeamNumber);
		MyPC->ClientMessage(FString::Printf(TEXT("Team changed to: %d"), MyPlayerState->GetTeamNum()));
	}
}

void UGameCheatManager::Cheat(const FString& Msg)
{
	GetOuterAGamePlayerController()->ServerCheat(Msg.Left(128));
}

void UGameCheatManager::SpawnBot()
{
	auto MyPC = GetOuterAGamePlayerController();
	auto MyPawn = MyPC->GetPawn();
	auto MyGame = MyPC->GetWorld()->GetAuthGameMode<ACoreGameModeBase>();
	auto World = MyPC->GetWorld();
	if (MyPawn && MyGame && World)
	{
		static int32 CheatBotNum = 50;
		AAIController* AIController = MyGame->CreateBot(CheatBotNum++);
		MyGame->RestartPlayer(AIController);		
	}
}