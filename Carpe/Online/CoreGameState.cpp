#include "CoreGameState.h"
#include "Carpe.h"
#include "Player/GamePlayerState.h"
#include "CoreGameInstance.h"
#include "Player/GamePlayerController.h"

ACoreGameState::ACoreGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NumTeams = 0;
	RemainingTime = 0;
	bTimerPaused = false;
}

void ACoreGameState::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( ACoreGameState, NumTeams );
	DOREPLIFETIME( ACoreGameState, RemainingTime );
	DOREPLIFETIME( ACoreGameState, bTimerPaused );
	DOREPLIFETIME( ACoreGameState, TeamScores );
}

void ACoreGameState::GetRankedMap(int32 TeamIndex, RankedPlayerMap2& OutRankedMap) const
{
	OutRankedMap.Empty();

	//first, we need to go over all the PlayerStates, grab their score, and rank them
	TMultiMap<int32, AGamePlayerState*> SortedMap;
	for(int32 i = 0; i < PlayerArray.Num(); ++i)
	{
		int32 Score = 0;
		auto CurPlayerState = Cast<AGamePlayerState>(PlayerArray[i]);
		if (CurPlayerState && (CurPlayerState->GetTeamNum() == TeamIndex))
		{
			SortedMap.Add(FMath::TruncToInt(CurPlayerState->Score), CurPlayerState);
		}
	}

	//sort by the keys
	SortedMap.KeySort(TGreater<int32>());

	//now, add them back to the ranked map
	OutRankedMap.Empty();

	int32 Rank = 0;
	for(TMultiMap<int32, AGamePlayerState*>::TIterator It(SortedMap); It; ++It)
	{
		OutRankedMap.Add(Rank++, It.Value());
	}
	
}


void ACoreGameState::RequestFinishAndExitToMainMenu()
{
	if (AuthorityGameMode)
	{
		// we are server, tell the gamemode
		auto GameMode = Cast<ACoreGameModeBase>(AuthorityGameMode);
		if (GameMode)
		{
			GameMode->RequestFinishAndExitToMainMenu();
		}
	}
	else
	{
		// we are client, handle our own business
		//UCoreGameInstance* GameInstance = Cast<UCoreGameInstance>(GetGameInstance());
		//if (GameInstance)
		//{
		//	GameInstance->RemoveSplitScreenPlayers();
		//}

		auto PrimaryPC = Cast<AGamePlayerController>(GetGameInstance()->GetFirstLocalPlayerController());
		if (PrimaryPC)
		{
			check(PrimaryPC->GetNetMode() == ENetMode::NM_Client);
			PrimaryPC->HandleReturnToMainMenu();
		}
	}

}
