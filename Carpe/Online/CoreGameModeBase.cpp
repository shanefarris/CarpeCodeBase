#include "Online/CoreGameModeBase.h"
#include "Carpe.h"
#include "CoreGameInstance.h"
//#include "UI/ShooterHUD.h"
#include "Player/GameSpectatorPawn.h"
#include "Player/Spectator.h"
#include "Player/GamePlayerState.h"
#include "Online/Session.h"
#include "AI/AIControllerBase.h"
#include "TeamStart.h"
#include "UI/CoreHudBase.h"
#include "Player/GamePlayerController.h"


ACoreGameModeBase::ACoreGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnOb(TEXT("Blueprint'/Game/Blueprints/Pawns/PlayerBP'"));
	DefaultPawnClass = PlayerPawnOb.Class; // Blueprint'/Game/Blueprints/Characters/FirstPersonCharacterBP.FirstPersonCharacterBP'

	static ConstructorHelpers::FClassFinder<APawn> BotPawnOb(TEXT("/Game/Blueprints/Pawns/BotPawn"));
	if(BotPawnOb.Succeeded())
		BotPawnClass = BotPawnOb.Class;

	// Blueprint'/Game/Blueprints/Inventory/Demo/JCInventoryHUD'
	//static ConstructorHelpers::FClassFinder<AHUD> hud(TEXT("Blueprint'/Game/UI/HUD/HUD.HUD'"));
	// Blueprint'/Game/UI/HUD/MainHud_HUD.MainHud_HUD'
	ConstructorHelpers::FClassFinder<AHUD> hud(TEXT("Blueprint'/Game/UI/HUD/MainHud_HUD'"));
	if (hud.Succeeded())
	{
		HUDClass = hud.Class;
	}
	else
	{
		HUDClass = ACoreHudBase::StaticClass();
	}

	PlayerControllerClass = AGamePlayerController::StaticClass();
	PlayerStateClass = AGamePlayerState::StaticClass();
	SpectatorClass = AGameSpectatorPawn::StaticClass();
	GameStateClass = ACoreGameState::StaticClass();
	ReplaySpectatorPlayerControllerClass = ASpectator::StaticClass();

	MinRespawnDelay = 5.0f;

	bAllowBots = true;
	bNeedsBotCreation = true;
	bUseSeamlessTravel = true;

	UE_LOG(LogCore, Log, TEXT("ACoreGameModeBase::ACoreGameModeBase"));
}

FString ACoreGameModeBase::GetBotsCountOptionName()
{
	return FString(TEXT("Bots"));
}

void ACoreGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	const int32 BotsCountOptionValue = UGameplayStatics::GetIntOption(Options, GetBotsCountOptionName(), 0);
	SetAllowBots(BotsCountOptionValue > 0 ? true : false, BotsCountOptionValue);	
	Super::InitGame(MapName, Options, ErrorMessage);

	//const UGameInstance* GameInstance = GetGameInstance();
	//if (GameInstance && Cast<UCoreGameInstance>(GameInstance)->GetIsOnline())
	//{
	//	bPauseable = false;
	//}
}

void ACoreGameModeBase::SetAllowBots(bool bInAllowBots, int32 InMaxBots)
{
	bAllowBots = bInAllowBots;
	MaxBots = InMaxBots;
}

/** Returns game session class to use */
TSubclassOf<AGameSession> ACoreGameModeBase::GetGameSessionClass() const
{
	return ASession::StaticClass();
}

void ACoreGameModeBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &ACoreGameModeBase::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);
}

void ACoreGameModeBase::DefaultTimer()
{
	// don't update timers for Play In Editor mode, it's not real match
	if (GetWorld()->IsPlayInEditor())
	{
		// start match if necessary.
		if (GetMatchState() == MatchState::WaitingToStart)
		{
			StartMatch();
		}
		return;
	}

	ACoreGameState* const MyGameState = Cast<ACoreGameState>(GameState);
	if (MyGameState && MyGameState->RemainingTime > 0 && !MyGameState->bTimerPaused)
	{
		MyGameState->RemainingTime--;
		
		if (MyGameState->RemainingTime <= 0)
		{
			if (GetMatchState() == MatchState::WaitingPostMatch)
			{
				RestartGame();
			}
			else if (GetMatchState() == MatchState::InProgress)
			{
				FinishMatch();

				// Send end round events
				for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
				{
					auto PlayerController = Cast<AGamePlayerController>(*It);
					
					if (PlayerController && MyGameState)
					{
						auto PlayerState = Cast<AGamePlayerState>((*It)->PlayerState);
						const bool bIsWinner = IsWinner(PlayerState);
					
						PlayerController->ClientSendRoundEndEvent(bIsWinner, MyGameState->ElapsedTime);
					}
				}
			}
			else if (GetMatchState() == MatchState::WaitingToStart)
			{
				StartMatch();
			}
		}
	}
}

void ACoreGameModeBase::HandleMatchIsWaitingToStart()
{
	if (bNeedsBotCreation)
	{
		CreateBotControllers();
		bNeedsBotCreation = false;
	}

	if (bDelayedStart)
	{
		// start warmup if needed
		ACoreGameState* const MyGameState = Cast<ACoreGameState>(GameState);
		if (MyGameState && MyGameState->RemainingTime == 0)
		{
			const bool bWantsMatchWarmup = !GetWorld()->IsPlayInEditor();
			if (bWantsMatchWarmup && WarmupTime > 0)
			{
				MyGameState->RemainingTime = WarmupTime;
			}
			else
			{
				MyGameState->RemainingTime = 0.0f;
			}
		}
	}
}

void ACoreGameModeBase::HandleMatchHasStarted()
{
	bNeedsBotCreation = true;
	Super::HandleMatchHasStarted();

	ACoreGameState* const MyGameState = Cast<ACoreGameState>(GameState);
	MyGameState->RemainingTime = RoundTime;	
	StartBots();	

	// notify players
	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		auto PC = Cast<AGamePlayerController>(*It);
		if (PC)
		{
			PC->ClientGameStarted();
		}
	}
}

void ACoreGameModeBase::FinishMatch()
{
	ACoreGameState* const MyGameState = Cast<ACoreGameState>(GameState);
	if (IsMatchInProgress())
	{
		EndMatch();
		DetermineMatchWinner();		

		// notify players
		for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
		{
			auto PlayerState = Cast<AGamePlayerState>((*It)->PlayerState);
			const bool bIsWinner = IsWinner(PlayerState);

			(*It)->GameHasEnded(nullptr, bIsWinner);
		}

		// lock all pawns
		// pawns are not marked as keep for seamless travel, so we will create new pawns on the next match rather than
		// turning these back on.
		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			(*It)->TurnOff();
		}

		// set up to restart the match
		MyGameState->RemainingTime = TimeBetweenMatches;
	}
}

void ACoreGameModeBase::RequestFinishAndExitToMainMenu()
{
	FinishMatch();

	//UCoreGameInstance* const GameInstance = Cast<UCoreGameInstance>(GetGameInstance());
	//if (GameInstance)
	//{
	//	GameInstance->RemoveSplitScreenPlayers();
	//}

	AGamePlayerController* LocalPrimaryController = nullptr;
	for (auto Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		auto Controller = Cast<AGamePlayerController>(*Iterator);

		if (Controller == NULL)
		{
			continue;
		}

		if (!Controller->IsLocalController())
		{
			const FString RemoteReturnReason = NSLOCTEXT("NetworkErrors", "HostHasLeft", "Host has left the game.").ToString();
			Controller->ClientReturnToMainMenu(RemoteReturnReason);
		}
		else
		{
			LocalPrimaryController = Controller;
		}
	}

	// GameInstance should be calling this from an EndState.  So call the PC function that performs cleanup, not the one that sets GI state.
	if (LocalPrimaryController != nullptr)
	{
		LocalPrimaryController->HandleReturnToMainMenu();
	}
}

void ACoreGameModeBase::DetermineMatchWinner()
{
	// nothing to do here
}

bool ACoreGameModeBase::IsWinner(class AGamePlayerState* PlayerState) const
{
	return false;
}

void ACoreGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	ACoreGameState* const MyGameState = Cast<ACoreGameState>(GameState);
	const bool bMatchIsOver = MyGameState && MyGameState->HasMatchEnded();
	if( bMatchIsOver )
	{
		ErrorMessage = TEXT("Match is over!");
	}
	else
	{
		// GameSession can be NULL if the match is over
		Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	}
}

void ACoreGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// update spectator location for client
	auto NewPC = Cast<AGamePlayerController>(NewPlayer);
	if (NewPC && NewPC->GetPawn() == nullptr)
	{
		NewPC->ClientSetSpectatorCamera(NewPC->GetSpawnLocation(), NewPC->GetControlRotation());
	}

	// notify new player if match is already in progress
	if (NewPC && IsMatchInProgress())
	{
		NewPC->ClientGameStarted();
		NewPC->ClientStartOnlineGame();
	}
}

void ACoreGameModeBase::Killed(AController* Killer, AController* KilledPlayer, APawn* KilledPawn, const UDamageType* DamageType)
{
	auto KillerPlayerState = Killer ? Cast<AGamePlayerState>(Killer->PlayerState) : nullptr;
	auto VictimPlayerState = KilledPlayer ? Cast<AGamePlayerState>(KilledPlayer->PlayerState) : nullptr;

	if (KillerPlayerState && KillerPlayerState != VictimPlayerState)
	{
		KillerPlayerState->ScoreKill(VictimPlayerState, KillScore);
		KillerPlayerState->InformAboutKill(KillerPlayerState, DamageType, VictimPlayerState);
	}

	if (VictimPlayerState)
	{
		VictimPlayerState->ScoreDeath(KillerPlayerState, DeathScore);
		VictimPlayerState->BroadcastDeath(KillerPlayerState, DamageType, VictimPlayerState);
	}
}

float ACoreGameModeBase::ModifyDamage(float Damage, AActor* DamagedActor, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) const
{
	float ActualDamage = Damage;

	ACharacterBase* DamagedPawn = Cast<ACharacterBase>(DamagedActor);
	if (DamagedPawn && EventInstigator)
	{
		auto DamagedPlayerState = Cast<AGamePlayerState>(DamagedPawn->PlayerState);
		auto InstigatorPlayerState = Cast<AGamePlayerState>(EventInstigator->PlayerState);

		// disable friendly fire
		if (!CanDealDamage(InstigatorPlayerState, DamagedPlayerState))
		{
			ActualDamage = 0.0f;
		}

		// scale self instigated damage
		if (InstigatorPlayerState == DamagedPlayerState)
		{
			ActualDamage *= DamageSelfScale;
		}
	}

	return ActualDamage;
}

bool ACoreGameModeBase::CanDealDamage(class AGamePlayerState* DamageInstigator, class AGamePlayerState* DamagedPlayer) const
{
	return true;
}

bool ACoreGameModeBase::AllowCheats(APlayerController* P)
{
	return true;
}

bool ACoreGameModeBase::ShouldSpawnAtStartSpot(AController* Player)
{
	return false;
}

UClass* ACoreGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (InController->IsA<AAIController>())
	{
		return BotPawnClass;
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

AActor* ACoreGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> PreferredSpawns;
	TArray<APlayerStart*> FallbackSpawns;

	APlayerStart* BestStart = NULL;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* TestSpawn = *It;
		if (TestSpawn->IsA<APlayerStartPIE>())
		{
			// Always prefer the first "Play from Here" PlayerStart, if we find one while in PIE mode
			BestStart = TestSpawn;
			break;
		}
		else
		{
			if (IsSpawnpointAllowed(TestSpawn, Player))
			{
				if (IsSpawnpointPreferred(TestSpawn, Player))
				{
					PreferredSpawns.Add(TestSpawn);
				}
				else
				{
					FallbackSpawns.Add(TestSpawn);
				}
			}
		}
	}

	
	if (BestStart == NULL)
	{
		if (PreferredSpawns.Num() > 0)
		{
			BestStart = PreferredSpawns[FMath::RandHelper(PreferredSpawns.Num())];
		}
		else if (FallbackSpawns.Num() > 0)
		{
			BestStart = FallbackSpawns[FMath::RandHelper(FallbackSpawns.Num())];
		}
	}

	return BestStart ? BestStart : Super::ChoosePlayerStart_Implementation(Player);
}

bool ACoreGameModeBase::IsSpawnpointAllowed(APlayerStart* SpawnPoint, AController* Player) const
{
	ATeamStart* ShooterSpawnPoint = Cast<ATeamStart>(SpawnPoint);
	if (ShooterSpawnPoint)
	{
		AAIController* AIController = Cast<AAIController>(Player);
		if (ShooterSpawnPoint->bNotForBots && AIController)
		{
			return false;
		}

		if (ShooterSpawnPoint->bNotForPlayers && AIController == NULL)
		{
			return false;
		}
		return true;
	}

	return false;
}

bool ACoreGameModeBase::IsSpawnpointPreferred(APlayerStart* SpawnPoint, AController* Player) const
{
	ACharacter* MyPawn = Cast<ACharacter>((*DefaultPawnClass)->GetDefaultObject<ACharacter>());	
	AAIController* AIController = Cast<AAIController>(Player);
	if( AIController != nullptr )
	{
		MyPawn = Cast<ACharacter>(BotPawnClass->GetDefaultObject<ACharacter>());
	}
	
	if (MyPawn)
	{
		const FVector SpawnLocation = SpawnPoint->GetActorLocation();
		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			ACharacter* OtherPawn = Cast<ACharacter>(*It);
			if (OtherPawn && OtherPawn != MyPawn)
			{
				const float CombinedHeight = (MyPawn->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + OtherPawn->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) * 2.0f;
				const float CombinedRadius = MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() + OtherPawn->GetCapsuleComponent()->GetScaledCapsuleRadius();
				const FVector OtherLocation = OtherPawn->GetActorLocation();

				// check if player start overlaps this pawn
				if (FMath::Abs(SpawnLocation.Z - OtherLocation.Z) < CombinedHeight && (SpawnLocation - OtherLocation).Size2D() < CombinedRadius)
				{
					return false;
				}
			}
		}
	}
	else
	{
		return false;
	}
	
	return true;
}

void ACoreGameModeBase::CreateBotControllers()
{
	UWorld* World = GetWorld();
	int32 ExistingBots = 0;
	for (FConstControllerIterator It = World->GetControllerIterator(); It; ++It)
	{		
		AAIController* AIC = Cast<AAIController>(*It);
		if (AIC)
		{
			++ExistingBots;
		}
	}

	// Create any necessary AIControllers.  Hold off on Pawn creation until pawns are actually necessary or need recreating.	
	int32 BotNum = ExistingBots;
	for (int32 i = 0; i < MaxBots - ExistingBots; ++i)
	{
		CreateBot(BotNum + i);
	}
}

AAIController* ACoreGameModeBase::CreateBot(int32 BotNum)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = nullptr;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.OverrideLevel = nullptr;

	UWorld* World = GetWorld();
	AAIController* AIC = World->SpawnActor<AAIController>(SpawnInfo);
	InitBot(AIC, BotNum);

	return AIC;
}

void ACoreGameModeBase::StartBots()
{
	// checking number of existing human player.
	UWorld* World = GetWorld();
	for (FConstControllerIterator It = World->GetControllerIterator(); It; ++It)
	{
		auto AIC = Cast<AAIController>(*It);
		if (AIC)
		{
			RestartPlayer(AIC);
		}
	}
}

void ACoreGameModeBase::InitBot(AAIController* AIController, int32 BotNum)
{	
	if (AIController)
	{
		if (AIController->PlayerState)
		{
			FString BotName = FString::Printf(TEXT("Bot %d"), BotNum);
			AIController->PlayerState->PlayerName = BotName;
		}		
	}
}

void ACoreGameModeBase::RestartGame()
{
	// Hide the scoreboard too !
	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = Cast<APlayerController>(*It);
		if (PlayerController != nullptr)
		{
			// TODO
			//AShooterHUD* ShooterHUD = Cast<AShooterHUD>(PlayerController->GetHUD());
			//if (ShooterHUD != nullptr)
			//{
			//	// Passing true to bFocus here ensures that focus is returned to the game viewport.
			//	ShooterHUD->ShowScoreboard(false, true);
			//}
		}
	}

	Super::RestartGame();
}

