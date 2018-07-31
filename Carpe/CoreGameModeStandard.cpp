#include "CoreGameModeStandard.h"
#include "Carpe.h"
#include "TeamStart.h"
#include "Online/CoreGameState.h"
#include "Player/GamePlayerState.h"
#include "AI/AIControllerBase.h"

ACoreGameModeStandard::ACoreGameModeStandard(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NumTeams = 2;
	bDelayedStart = false;
}

void ACoreGameModeStandard::PostLogin(APlayerController* NewPlayer)
{
	// Place player on a team before Super (VoIP team based init, findplayerstart, etc)
	auto NewPlayerState = CastChecked<AGamePlayerState>(NewPlayer->PlayerState);
	const auto TeamNum = ChooseTeam(NewPlayerState);
	NewPlayerState->SetTeamNum(TeamNum);

	Super::PostLogin(NewPlayer);
}

void ACoreGameModeStandard::InitGameState()
{
	Super::InitGameState();

	auto const MyGameState = Cast<ACoreGameState>(GameState);
	if (MyGameState)
	{
		MyGameState->NumTeams = NumTeams;
	}
}

bool ACoreGameModeStandard::CanDealDamage(AGamePlayerState* DamageInstigator, class AGamePlayerState* DamagedPlayer) const
{
	return DamageInstigator && DamagedPlayer && 
		(DamagedPlayer == DamageInstigator || DamagedPlayer->GetTeamNum() != DamageInstigator->GetTeamNum());
}

int32 ACoreGameModeStandard::ChooseTeam(AGamePlayerState* ForPlayerState) const
{
	TArray<int32> TeamBalance;
	TeamBalance.AddZeroed(NumTeams);

	// Get current team balance
	for (auto playerState : GameState->PlayerArray)
	{
		auto TestPlayerState = Cast<AGamePlayerState>(playerState);
		if (TestPlayerState && 
			TestPlayerState != ForPlayerState && 
			TeamBalance.IsValidIndex(TestPlayerState->GetTeamNum()))
		{
			TeamBalance[TestPlayerState->GetTeamNum()]++;
		}
	}

	// Find least populated one
	auto BestTeamScore = TeamBalance[0];
	for (auto i : TeamBalance)
	{
		if (BestTeamScore > i)
		{
			BestTeamScore = i;
		}
	}

	// There could be more than one...
	TArray<int32> BestTeams;
	for (auto i = 0; i < TeamBalance.Num(); i++)
	{
		if (TeamBalance[i] == BestTeamScore)
		{
			BestTeams.Add(i);
		}
	}

	// Get random from best list
	const auto RandomBestTeam = BestTeams[FMath::RandHelper(BestTeams.Num())];
	return RandomBestTeam;
}

bool ACoreGameModeStandard::IsSpawnpointAllowed(APlayerStart* SpawnPoint, AController* Player) const
{
	if (Player)
	{
		auto TeamStart = Cast<ATeamStart>(SpawnPoint);
		auto PlayerState = Cast<AGamePlayerState>(Player->PlayerState);

		if (PlayerState && TeamStart && TeamStart->SpawnTeam != PlayerState->GetTeamNum())
		{
			return false;
		}
	}

	return Super::IsSpawnpointAllowed(SpawnPoint, Player);
}

void ACoreGameModeStandard::InitBot(AAIController* AIC, int32 BotNum)
{
	auto BotPlayerState = CastChecked<AGamePlayerState>(AIC->PlayerState);
	const int32 TeamNum = ChooseTeam(BotPlayerState);
	BotPlayerState->SetTeamNum(TeamNum);

	Super::InitBot(AIC, BotNum);
}