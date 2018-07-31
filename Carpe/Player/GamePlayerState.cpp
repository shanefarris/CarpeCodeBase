#include "GamePlayerState.h"
#include "Carpe.h"

AGamePlayerState::AGamePlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TeamNumber = 0;
	NumKills = 0;
	NumDeaths = 0;
	NumBulletsFired = 0;
	NumRocketsFired = 0;
	bQuitter = false;
}

void AGamePlayerState::Reset()
{
	Super::Reset();
	
	//PlayerStates persist across seamless travel.  Keep the same teams as previous match.
	//SetTeamNum(0);
	NumKills = 0;
	NumDeaths = 0;
	NumBulletsFired = 0;
	NumRocketsFired = 0;
	bQuitter = false;
}

void AGamePlayerState::UnregisterPlayerWithSession()
{
	if (!bFromPreviousLevel)
	{
		Super::UnregisterPlayerWithSession();
	}
}

void AGamePlayerState::ClientInitialize(AController* InController)
{
	Super::ClientInitialize(InController);

	UpdateTeamColors();
}

void AGamePlayerState::SetTeamNum(int32 NewTeamNumber)
{
	TeamNumber = NewTeamNumber;

	UpdateTeamColors();
}

void AGamePlayerState::OnRep_TeamColor()
{
	UpdateTeamColors();
}

void AGamePlayerState::AddBulletsFired(int32 NumBullets)
{
	NumBulletsFired += NumBullets;
}

void AGamePlayerState::AddRocketsFired(int32 NumRockets)
{
	NumRocketsFired += NumRockets;
}

void AGamePlayerState::SetQuitter(bool bInQuitter)
{
	bQuitter = bInQuitter;
}

void AGamePlayerState::CopyProperties(APlayerState* PlayerState)
{	
	Super::CopyProperties(PlayerState);

	auto ShooterPlayer = Cast<AGamePlayerState>(PlayerState);
	if (ShooterPlayer)
	{
		ShooterPlayer->TeamNumber = TeamNumber;
	}	
}

void AGamePlayerState::UpdateTeamColors()
{
	AController* OwnerController = Cast<AController>(GetOwner());
	if (OwnerController != nullptr)
	{
		ACharacterBase* Character = Cast<ACharacterBase>(OwnerController->GetCharacter());
		if (Character != nullptr)
		{
			Character->UpdateTeamColorsAllMIDs();
		}
	}
}

int32 AGamePlayerState::GetTeamNum() const
{
	return TeamNumber;
}

int32 AGamePlayerState::GetKills() const
{
	return NumKills;
}

int32 AGamePlayerState::GetDeaths() const
{
	return NumDeaths;
}

float AGamePlayerState::GetScore() const
{
	return Score;
}

int32 AGamePlayerState::GetNumBulletsFired() const
{
	return NumBulletsFired;
}

int32 AGamePlayerState::GetNumRocketsFired() const
{
	return NumRocketsFired;
}

bool AGamePlayerState::IsQuitter() const
{
	return bQuitter;
}

void AGamePlayerState::ScoreKill(APlayerState* Victim, int32 Points)
{
	NumKills++;
	ScorePoints(Points);
}

void AGamePlayerState::ScoreDeath(APlayerState* KilledBy, int32 Points)
{
	NumDeaths++;
	ScorePoints(Points);
}

void AGamePlayerState::ScorePoints(int32 Points)
{
	ACoreGameState* const MyGameState = Cast<ACoreGameState>(GetWorld()->GetGameState());
	if (MyGameState && TeamNumber >= 0)
	{
		if (TeamNumber >= MyGameState->TeamScores.Num())
		{
			MyGameState->TeamScores.AddZeroed(TeamNumber - MyGameState->TeamScores.Num() + 1);
		}

		MyGameState->TeamScores[TeamNumber] += Points;
	}

	Score += Points;
}

void AGamePlayerState::InformAboutKill_Implementation(class AGamePlayerState* KillerPlayerState, const UDamageType* KillerDamageType, class AGamePlayerState* KilledPlayerState)
{
	//id can be null for bots
	if (KillerPlayerState->UniqueId.IsValid())
	{	
		//search for the actual killer before calling OnKill()	
		for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{		
			auto TestPC = Cast<AGamePlayerController>(*It);
			if (TestPC && TestPC->IsLocalController())
			{
				// a local player might not have an ID if it was created with CreateDebugPlayer.
				auto LocalPlayer = Cast<ULocalPlayer>(TestPC->Player);
				auto LocalID = LocalPlayer->GetCachedUniqueNetId();
				if (LocalID.IsValid() &&  *LocalPlayer->GetCachedUniqueNetId() == *KillerPlayerState->UniqueId)
				{			
					TestPC->OnKill();
				}
			}
		}
	}
}

void AGamePlayerState::BroadcastDeath_Implementation(class AGamePlayerState* KillerPlayerState, const UDamageType* KillerDamageType, class AGamePlayerState* KilledPlayerState)
{	
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		// all local players get death messages so they can update their huds.
		auto TestPC = Cast<AGamePlayerController>(*It);
		if (TestPC && TestPC->IsLocalController())
		{
			TestPC->OnDeathMessage(KillerPlayerState, this, KillerDamageType);				
		}
	}	
}

void AGamePlayerState::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( AGamePlayerState, TeamNumber );
	DOREPLIFETIME(AGamePlayerState, NumKills );
	DOREPLIFETIME(AGamePlayerState, NumDeaths );
}

FString AGamePlayerState::GetShortPlayerName() const
{
	if( PlayerName.Len() > MAX_PLAYER_NAME_LENGTH )
	{
		return PlayerName.Left(MAX_PLAYER_NAME_LENGTH) + "...";
	}
	return PlayerName;
}
