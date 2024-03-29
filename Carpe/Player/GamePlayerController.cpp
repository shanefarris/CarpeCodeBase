#include "Player/GamePlayerController.h"
#include "Carpe.h"
#include "Player/GameCameraManager.h"
#include "Player/GameCheatManager.h"
#include "Player/GameLocalPlayer.h"
#include "Player/GamePlayerState.h"
#include "Inventory/Weapon.h"
//#include "UI/Menu/ShooterIngameMenu.h"
//#include "UI/Style/ShooterStyle.h"
//#include "UI/ShooterHUD.h"
#include "Online.h"
#include "OnlineAchievementsInterface.h"
#include "OnlineEventsInterface.h"
#include "OnlineIdentityInterface.h"
#include "OnlineSessionInterface.h"
#include "CoreGameInstance.h"
#include "Leaderboards.h"
#include "ViewportClient.h"


#define  ACH_FRAG_SOMEONE	TEXT("ACH_FRAG_SOMEONE")
#define  ACH_SOME_KILLS		TEXT("ACH_SOME_KILLS")
#define  ACH_LOTS_KILLS		TEXT("ACH_LOTS_KILLS")
#define  ACH_FINISH_MATCH	TEXT("ACH_FINISH_MATCH")
#define  ACH_LOTS_MATCHES	TEXT("ACH_LOTS_MATCHES")
#define  ACH_FIRST_WIN		TEXT("ACH_FIRST_WIN")
#define  ACH_LOTS_WIN		TEXT("ACH_LOTS_WIN")
#define  ACH_MANY_WIN		TEXT("ACH_MANY_WIN")
#define  ACH_SHOOT_BULLETS	TEXT("ACH_SHOOT_BULLETS")
#define  ACH_SHOOT_ROCKETS	TEXT("ACH_SHOOT_ROCKETS")
#define  ACH_GOOD_SCORE		TEXT("ACH_GOOD_SCORE")
#define  ACH_GREAT_SCORE	TEXT("ACH_GREAT_SCORE")
#define  ACH_PLAY_SANCTUARY	TEXT("ACH_PLAY_SANCTUARY")
#define  ACH_PLAY_HIGHRISE	TEXT("ACH_PLAY_HIGHRISE")

static const int32 SomeKillsCount = 10;
static const int32 LotsKillsCount = 20;
static const int32 LotsMatchesCount = 5;
static const int32 LotsWinsCount = 3;
static const int32 ManyWinsCount = 5;
static const int32 LotsBulletsCount = 100;
static const int32 LotsRocketsCount = 10;
static const int32 GoodScoreCount = 10;
static const int32 GreatScoreCount = 15;

AGamePlayerController::AGamePlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AGameCameraManager::StaticClass();
	CheatClass = UGameCheatManager::StaticClass();
	bAllowGameActions = true;
	bGameEndedFrame = false;
	LastDeathLocation = FVector::ZeroVector;

	ServerSayString = TEXT("Say");
	ShooterFriendUpdateTimer = 0.0f;
	bHasSentStartEvents = false;
}

void AGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// UI input
	InputComponent->BindAction("InGameMenu", IE_Pressed, this, &AGamePlayerController::OnToggleInGameMenu);
	InputComponent->BindAction("Scoreboard", IE_Pressed, this, &AGamePlayerController::OnShowScoreboard);
	InputComponent->BindAction("Scoreboard", IE_Released, this, &AGamePlayerController::OnHideScoreboard);
	InputComponent->BindAction("ConditionalCloseScoreboard", IE_Pressed, this, &AGamePlayerController::OnConditionalCloseScoreboard);
	InputComponent->BindAction("ToggleScoreboard", IE_Pressed, this, &AGamePlayerController::OnToggleScoreboard);

	// voice chat
	InputComponent->BindAction("PushToTalk", IE_Pressed, this, &AGamePlayerController::StartTalking);
	InputComponent->BindAction("PushToTalk", IE_Released, this, &AGamePlayerController::StopTalking);

	InputComponent->BindAction("ToggleChat", IE_Pressed, this, &AGamePlayerController::ToggleChatWindow);
}


void AGamePlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// TODO
	//FShooterStyle::Initialize();
	ShooterFriendUpdateTimer = 0;
}

void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();	
}

void AGamePlayerController::TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	if (IsGameMenuVisible())
	{
		if (ShooterFriendUpdateTimer > 0)
		{
			ShooterFriendUpdateTimer -= DeltaTime;
		}
		else
		{
			// TODO
			//TSharedPtr<class FShooterFriends> ShooterFriends = ShooterIngameMenu->GetShooterFriends();
			//ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
			//if (ShooterFriends.IsValid() && LocalPlayer && LocalPlayer->GetControllerId() >= 0)
			//{
			//	ShooterFriends->UpdateFriends(LocalPlayer->GetControllerId());
			//}
			//ShooterFriendUpdateTimer = 4; //make sure the time between calls is long enough that we won't trigger (0x80552C81) and not exceed the web api rate limit
		}
	}

	// Is this the first frame after the game has ended
	if(bGameEndedFrame)
	{
		bGameEndedFrame = false;

		// ONLY PUT CODE HERE WHICH YOU DON'T WANT TO BE DONE DUE TO HOST LOSS

		// Do we need to show the end of round scoreboard?
		if (IsPrimaryPlayer())
		{
			// TODO
			//AShooterHUD* ShooterHUD = GetShooterHUD();
			//if (ShooterHUD)
			//{
			//	ShooterHUD->ShowScoreboard(true, true);
			//}
		}
	}
};

void AGamePlayerController::SetPlayer( UPlayer* InPlayer )
{
	Super::SetPlayer( InPlayer );

	// TODO
	//Build menu only after game is initialized
	//ShooterIngameMenu = MakeShareable(new FShooterIngameMenu());
	//ShooterIngameMenu->Construct(Cast<ULocalPlayer>(Player));

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AGamePlayerController::QueryAchievements()
{
	// precache achievements
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	if (LocalPlayer && LocalPlayer->GetControllerId() != -1)
	{
		IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
		if(OnlineSub)
		{
			IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
			if (Identity.IsValid())
			{
				TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(LocalPlayer->GetControllerId());

				if (UserId.IsValid())
				{
					IOnlineAchievementsPtr Achievements = OnlineSub->GetAchievementsInterface();

					if (Achievements.IsValid())
					{
						Achievements->QueryAchievements( *UserId.Get(), FOnQueryAchievementsCompleteDelegate::CreateUObject( this, &AGamePlayerController::OnQueryAchievementsComplete ));
					}
				}
				else
				{
					UE_LOG(LogOnline, Warning, TEXT("No valid user id for this controller."));
				}
			}
			else
			{
				UE_LOG(LogOnline, Warning, TEXT("No valid identity interface."));
			}
		}
		else
		{
			UE_LOG(LogOnline, Warning, TEXT("No default online subsystem."));
		}
	}
	else
	{
		UE_LOG(LogOnline, Warning, TEXT("No local player, cannot read achievements."));
	}
}

void AGamePlayerController::OnQueryAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful )
{
	UE_LOG(LogOnline, Display, TEXT("AGamePlayerController::OnQueryAchievementsComplete(bWasSuccessful = %s)"), bWasSuccessful ? TEXT("TRUE") : TEXT("FALSE"));
}

void AGamePlayerController::UnFreeze()
{
	//ServerRestartPlayer(); Don't respawn

	UGameplayStatics::OpenLevel(GetWorld(), "MainEntry");
}

void AGamePlayerController::FailedToSpawnPawn()
{
	if(StateName == NAME_Inactive)
	{
		BeginInactiveState();
	}
	Super::FailedToSpawnPawn();
}

void AGamePlayerController::PawnPendingDestroy(APawn* P)
{
	LastDeathLocation = P->GetActorLocation();
	FVector CameraLocation = LastDeathLocation + FVector(0, 0, 300.0f);
	FRotator CameraRotation(-90.0f, 0.0f, 0.0f);
	FindDeathCameraSpot(CameraLocation, CameraRotation);

	Super::PawnPendingDestroy(P);

	ClientSetSpectatorCamera(CameraLocation, CameraRotation);
}

void AGamePlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	UpdateSaveFileOnGameEnd(bIsWinner);
	UpdateAchievementsOnGameEnd();
	UpdateLeaderboardsOnGameEnd();

	Super::GameHasEnded(EndGameFocus, bIsWinner);
}

void AGamePlayerController::ClientSetSpectatorCamera_Implementation(FVector CameraLocation, FRotator CameraRotation)
{
	SetInitialLocationAndRotation(CameraLocation, CameraRotation);
	SetViewTarget(this);
}

bool AGamePlayerController::FindDeathCameraSpot(FVector& CameraLocation, FRotator& CameraRotation)
{
	const FVector PawnLocation = GetPawn()->GetActorLocation();
	FRotator ViewDir = GetControlRotation();
	ViewDir.Pitch = -45.0f;

	const float YawOffsets[] = { 0.0f, -180.0f, 90.0f, -90.0f, 45.0f, -45.0f, 135.0f, -135.0f };
	const float CameraOffset = 600.0f;
	FCollisionQueryParams TraceParams(TEXT("DeathCamera"), true, GetPawn());

	FHitResult HitResult;
	for (int32 i = 0; i < ARRAY_COUNT(YawOffsets); i++)
	{
		FRotator CameraDir = ViewDir;
		CameraDir.Yaw += YawOffsets[i];
		CameraDir.Normalize();

		const FVector TestLocation = PawnLocation - CameraDir.Vector() * CameraOffset;
		
		const bool bBlocked = GetWorld()->LineTraceSingleByChannel(HitResult, PawnLocation, TestLocation, ECC_Camera, TraceParams);

		if (!bBlocked)
		{
			CameraLocation = TestLocation;
			CameraRotation = CameraDir;
			return true;
		}
	}

	return false;
}

bool AGamePlayerController::ServerCheat_Validate(const FString& Msg)
{
	return true;
}

void AGamePlayerController::ServerCheat_Implementation(const FString& Msg)
{
	if (CheatManager)
	{
		ClientMessage(ConsoleCommand(Msg));
	}
}

void AGamePlayerController::SimulateInputKey(FKey Key, bool bPressed)
{
	InputKey(Key, bPressed ? IE_Pressed : IE_Released, 1, false);
}

void AGamePlayerController::OnKill()
{
	UpdateAchievementProgress(ACH_FRAG_SOMEONE, 100.0f);

	const auto Events = Online::GetEventsInterface();
	const auto Identity = Online::GetIdentityInterface();

	if (Events.IsValid() && Identity.IsValid())
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
		if (LocalPlayer)
		{
			int32 UserIndex = LocalPlayer->GetControllerId();
			TSharedPtr<const FUniqueNetId> UniqueID = Identity->GetUniquePlayerId(UserIndex);			
			if (UniqueID.IsValid())
			{			
				ACharacterBase* pawn = Cast<ACharacterBase>(GetCharacter());
				// If player is dead, use location stored during pawn cleanup.
				FVector Location = pawn ? pawn->GetActorLocation() : LastDeathLocation;
				AWeapon* Weapon = pawn ? pawn->GetWeapon() : 0;
				int32 WeaponType = Weapon ? (int32)Weapon->GetAmmoType() : 0;

				FOnlineEventParms Params;		

				Params.Add( TEXT( "SectionId" ), FVariantData( (int32)0 ) ); // unused
				Params.Add( TEXT( "GameplayModeId" ), FVariantData( (int32)1 ) ); // @todo determine game mode (ffa v tdm)
				Params.Add( TEXT( "DifficultyLevelId" ), FVariantData( (int32)0 ) ); // unused

				Params.Add( TEXT( "PlayerRoleId" ), FVariantData( (int32)0 ) ); // unused
				Params.Add( TEXT( "PlayerWeaponId" ), FVariantData( (int32)WeaponType ) );
				Params.Add( TEXT( "EnemyRoleId" ), FVariantData( (int32)0 ) ); // unused
				Params.Add( TEXT( "EnemyWeaponId" ), FVariantData( (int32)0 ) ); // untracked			
				Params.Add( TEXT( "KillTypeId" ), FVariantData( (int32)0 ) ); // unused
				Params.Add( TEXT( "LocationX" ), FVariantData( Location.X ) );
				Params.Add( TEXT( "LocationY" ), FVariantData( Location.Y ) );
				Params.Add( TEXT( "LocationZ" ), FVariantData( Location.Z ) );
			
				Events->TriggerEvent(*UniqueID, TEXT("KillOponent"), Params);				
			}
		}
	}
}

void AGamePlayerController::OnDeathMessage(class AGamePlayerState* KillerPlayerState, class AGamePlayerState* KilledPlayerState, const UDamageType* KillerDamageType)
{
	// TODO
	//AShooterHUD* ShooterHUD = GetShooterHUD();
	//if (ShooterHUD)
	//{
	//	ShooterHUD->ShowDeathMessage(KillerPlayerState, KilledPlayerState, KillerDamageType);		
	//}

	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	if (LocalPlayer && LocalPlayer->GetCachedUniqueNetId().IsValid() && KilledPlayerState->UniqueId.IsValid())
	{
		// if this controller is the player who died, update the hero stat.
		if (*LocalPlayer->GetCachedUniqueNetId() == *KilledPlayerState->UniqueId)
		{
			const auto Events = Online::GetEventsInterface();
			const auto Identity = Online::GetIdentityInterface();

			if (Events.IsValid() && Identity.IsValid())
			{							
				const int32 UserIndex = LocalPlayer->GetControllerId();
				TSharedPtr<const FUniqueNetId> UniqueID = Identity->GetUniquePlayerId(UserIndex);
				if (UniqueID.IsValid())
				{				
					auto ShooterChar = Cast<ACharacterBase>(GetCharacter());
					auto Weapon = ShooterChar ? ShooterChar->GetWeapon() : nullptr;

					FVector Location = ShooterChar ? ShooterChar->GetActorLocation() : FVector::ZeroVector;
					int32 WeaponType = Weapon ? (int32)Weapon->GetAmmoType() : 0;

					FOnlineEventParms Params;
					Params.Add(TEXT("SectionId"), FVariantData((int32)0)); // unused
					Params.Add(TEXT("GameplayModeId"), FVariantData((int32)1)); // @todo determine game mode (ffa v tdm)
					Params.Add(TEXT("DifficultyLevelId"), FVariantData((int32)0)); // unused

					Params.Add(TEXT("PlayerRoleId"), FVariantData((int32)0)); // unused
					Params.Add(TEXT("PlayerWeaponId"), FVariantData((int32)WeaponType));
					Params.Add(TEXT("EnemyRoleId"), FVariantData((int32)0)); // unused
					Params.Add(TEXT("EnemyWeaponId"), FVariantData((int32)0)); // untracked

					Params.Add(TEXT("LocationX"), FVariantData(Location.X));
					Params.Add(TEXT("LocationY"), FVariantData(Location.Y));
					Params.Add(TEXT("LocationZ"), FVariantData(Location.Z));

					Events->TriggerEvent(*UniqueID, TEXT("PlayerDeath"), Params);
				}
			}
		}
	}	
}

void AGamePlayerController::UpdateAchievementProgress( const FString& Id, float Percent )
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	if (LocalPlayer)
	{
		IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
		if(OnlineSub)
		{
			IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
			if (Identity.IsValid())
			{
				TSharedPtr<const FUniqueNetId> UserId = LocalPlayer->GetCachedUniqueNetId();

				if (UserId.IsValid())
				{

					IOnlineAchievementsPtr Achievements = OnlineSub->GetAchievementsInterface();
					if (Achievements.IsValid() && (!WriteObject.IsValid() || WriteObject->WriteState != EOnlineAsyncTaskState::InProgress))
					{
						WriteObject = MakeShareable(new FOnlineAchievementsWrite());
						WriteObject->SetFloatStat(*Id, Percent);

						FOnlineAchievementsWriteRef WriteObjectRef = WriteObject.ToSharedRef();
						Achievements->WriteAchievements(*UserId, WriteObjectRef);
					}
					else
					{
						UE_LOG(LogOnline, Warning, TEXT("No valid achievement interface or another write is in progress."));
					}
				}
				else
				{
					UE_LOG(LogOnline, Warning, TEXT("No valid user id for this controller."));
				}
			}
			else
			{
				UE_LOG(LogOnline, Warning, TEXT("No valid identity interface."));
			}
		}
		else
		{
			UE_LOG(LogOnline, Warning, TEXT("No default online subsystem."));
		}
	}
	else
	{
		UE_LOG(LogOnline, Warning, TEXT("No local player, cannot update achievements."));
	}
}

void AGamePlayerController::OnToggleInGameMenu()
{
	// this is not ideal, but necessary to prevent both players from pausing at the same time on the same frame
	UWorld* GameWorld = GEngine->GameViewport->GetWorld();

	for(auto It = GameWorld->GetControllerIterator(); It; ++It)
	{
		AGamePlayerController* Controller = Cast<AGamePlayerController>(*It);
		if(Controller && Controller->IsPaused())
		{
			return;
		}
	}

	// TODO
	// if no one's paused, pause
	//if (ShooterIngameMenu.IsValid())
	//{
	//	ShooterIngameMenu->ToggleGameMenu();
	//}
}

void AGamePlayerController::OnConditionalCloseScoreboard()
{
	// TODO
	//AShooterHUD* ShooterHUD = GetShooterHUD();
	//if(ShooterHUD && ( ShooterHUD->IsMatchOver() == false ))
	//{
	//	ShooterHUD->ConditionalCloseScoreboard();
	//}
}

void AGamePlayerController::OnToggleScoreboard()
{
	// TODO
	//AShooterHUD* ShooterHUD = GetShooterHUD();
	//if(ShooterHUD && ( ShooterHUD->IsMatchOver() == false ))
	//{
	//	ShooterHUD->ToggleScoreboard();
	//}
}

void AGamePlayerController::OnShowScoreboard()
{
	// TODO
	//AShooterHUD* ShooterHUD = GetShooterHUD();
	//if(ShooterHUD)
	//{
	//	ShooterHUD->ShowScoreboard(true);
	//}
}

void AGamePlayerController::OnHideScoreboard()
{
	// TODO
	//AShooterHUD* ShooterHUD = GetShooterHUD();
	//// If have a valid match and the match is over - hide the scoreboard
	//if( (ShooterHUD != NULL ) && ( ShooterHUD->IsMatchOver() == false ) )
	//{
	//	ShooterHUD->ShowScoreboard(false);
	//}
}

bool AGamePlayerController::IsGameMenuVisible() const
{
	// TODO
	bool Result = false; 
	//if (ShooterIngameMenu.IsValid())
	//{
	//	Result = ShooterIngameMenu->GetIsGameMenuUp();
	//} 

	return Result;
}

void AGamePlayerController::SetInfiniteAmmo(bool bEnable)
{
	bInfiniteAmmo = bEnable;
}

void AGamePlayerController::SetInfiniteClip(bool bEnable)
{
	bInfiniteClip = bEnable;
}

void AGamePlayerController::SetHealthRegen(bool bEnable)
{
	bHealthRegen = bEnable;
}

void AGamePlayerController::SetGodMode(bool bEnable)
{
	bGodMode = bEnable;
}

void AGamePlayerController::ClientGameStarted_Implementation()
{
	bAllowGameActions = true;

	// Enable controls mode now the game has started
	SetIgnoreMoveInput(false);

	// TODO
	//AShooterHUD* ShooterHUD = GetShooterHUD();
	//if (ShooterHUD)
	//{
	//	ShooterHUD->SetMatchState(EMatchState::Playing);
	//	ShooterHUD->ShowScoreboard(false);
	//}
	bGameEndedFrame = false;

	QueryAchievements();

	// Send round start event
	const auto Events = Online::GetEventsInterface();
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);

	if(LocalPlayer != nullptr && Events.IsValid())
	{
		auto UniqueId = LocalPlayer->GetPreferredUniqueNetId();

		if (UniqueId.IsValid())
		{
			// Generate a new session id
			Events->SetPlayerSessionId(*UniqueId, FGuid::NewGuid());

			FString MapName = *FPackageName::GetShortName(GetWorld()->PersistentLevel->GetOutermost()->GetName());

			// Fire session start event for all cases
			FOnlineEventParms Params;
			Params.Add( TEXT( "GameplayModeId" ), FVariantData( (int32)1 ) ); // @todo determine game mode (ffa v tdm)
			Params.Add( TEXT( "DifficultyLevelId" ), FVariantData( (int32)0 ) ); // unused
			Params.Add( TEXT( "MapName" ), FVariantData( MapName ) );
			
			Events->TriggerEvent(*UniqueId, TEXT("PlayerSessionStart"), Params);

			// Online matches require the MultiplayerRoundStart event as well
			//UCoreGameInstance* SGI = GetWorld() != NULL ? Cast<UCoreGameInstance>(GetWorld()->GetGameInstance()) : NULL;

			//if (SGI->GetIsOnline())
			//{
			//	FOnlineEventParms MultiplayerParams;

			//	// @todo: fill in with real values
			//	MultiplayerParams.Add( TEXT( "SectionId" ), FVariantData( (int32)0 ) ); // unused
			//	MultiplayerParams.Add( TEXT( "GameplayModeId" ), FVariantData( (int32)1 ) ); // @todo determine game mode (ffa v tdm)
			//	MultiplayerParams.Add( TEXT( "MatchTypeId" ), FVariantData( (int32)1 ) ); // @todo abstract the specific meaning of this value across platforms
			//	MultiplayerParams.Add( TEXT( "DifficultyLevelId" ), FVariantData( (int32)0 ) ); // unused
			//	
			//	Events->TriggerEvent(*UniqueId, TEXT("MultiplayerRoundStart"), MultiplayerParams);
			//}

			bHasSentStartEvents = true;
		}
	}
}

/** Starts the online game using the session name in the PlayerState */
void AGamePlayerController::ClientStartOnlineGame_Implementation()
{
	if (!IsPrimaryPlayer())
		return;

	APlayerState* ShooterPlayerState = Cast<AGamePlayerState>(PlayerState);
	if (ShooterPlayerState)
	{
		IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
		if (OnlineSub)
		{
			IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
			if (Sessions.IsValid())
			{
				UE_LOG(LogOnline, Log, TEXT("Starting session %s on client"), *ShooterPlayerState->SessionName.ToString() );
				Sessions->StartSession(ShooterPlayerState->SessionName);
			}
		}
	}
	else
	{
		// Keep retrying until player state is replicated
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ClientStartOnlineGame, this, &AGamePlayerController::ClientStartOnlineGame_Implementation, 0.2f, false);
	}
}

/** Ends the online game using the session name in the PlayerState */
void AGamePlayerController::ClientEndOnlineGame_Implementation()
{
	if (!IsPrimaryPlayer())
		return;

	APlayerState* ShooterPlayerState = Cast<AGamePlayerState>(PlayerState);
	if (ShooterPlayerState)
	{
		IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
		if (OnlineSub)
		{
			IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
			if (Sessions.IsValid())
			{
				UE_LOG(LogOnline, Log, TEXT("Ending session %s on client"), *ShooterPlayerState->SessionName.ToString() );
				Sessions->EndSession(ShooterPlayerState->SessionName);
			}
		}
	}
}

void AGamePlayerController::HandleReturnToMainMenu()
{
	OnHideScoreboard();
	CleanupSessionOnReturnToMenu();
}

void AGamePlayerController::ClientReturnToMainMenu_Implementation(const FString& InReturnReason)
{		
	//UCoreGameInstance* SGI = GetWorld() != NULL ? Cast<UCoreGameInstance>(GetWorld()->GetGameInstance()) : NULL;

	//if ( !ensure( SGI != NULL ) )
	//{
	//	return;
	//}

	//if ( GetNetMode() == NM_Client )
	//{
	//	const FText ReturnReason	= NSLOCTEXT( "NetworkErrors", "HostQuit", "The host has quit the match." );
	//	const FText OKButton		= NSLOCTEXT( "DialogButtons", "OKAY", "OK" );

	//	SGI->ShowMessageThenGotoState( ReturnReason, OKButton, FText::GetEmpty(), CoreGameInstanceState::MainMenu );
	//}
	//else
	//{
	//	SGI->GotoState(CoreGameInstanceState::MainMenu);
	//}

	// Clear the flag so we don't do normal end of round stuff next
	bGameEndedFrame = false;
}

/** Ends and/or destroys game session */
void AGamePlayerController::CleanupSessionOnReturnToMenu()
{
	//UCoreGameInstance * SGI = GetWorld() != NULL ? Cast<UCoreGameInstance>( GetWorld()->GetGameInstance() ) : NULL;

	//if ( ensure( SGI != NULL ) )
	//{
	//	SGI->CleanupSessionOnReturnToMenu();
	//}
}

void AGamePlayerController::ClientGameEnded_Implementation(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::ClientGameEnded_Implementation(EndGameFocus, bIsWinner);
	
	// Disable controls now the game has ended
	SetIgnoreMoveInput(true);

	bAllowGameActions = false;

	// Make sure that we still have valid view target
	SetViewTarget(GetPawn());

	// TODO
	//AShooterHUD* ShooterHUD = GetShooterHUD();
	//if (ShooterHUD)
	//{
	//	ShooterHUD->SetMatchState(bIsWinner ? EMatchState::Won : EMatchState::Lost);
	//}

	UpdateSaveFileOnGameEnd(bIsWinner);
	UpdateAchievementsOnGameEnd();
	UpdateLeaderboardsOnGameEnd();

	// Flag that the game has just ended (if it's ended due to host loss we want to wait for ClientReturnToMainMenu_Implementation first, incase we don't want to process)
	bGameEndedFrame = true;
}

void AGamePlayerController::ClientSendRoundEndEvent_Implementation(bool bIsWinner, int32 ExpendedTimeInSeconds)
{
	const auto Events = Online::GetEventsInterface();
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);

	if(bHasSentStartEvents && LocalPlayer != nullptr && Events.IsValid())
	{	
		auto UniqueId = LocalPlayer->GetPreferredUniqueNetId();

		if (UniqueId.IsValid())
		{
			auto MapName = *FPackageName::GetShortName(GetWorld()->PersistentLevel->GetOutermost()->GetName());
			auto ShooterPlayerState = Cast<AGamePlayerState>(PlayerState);
			int32 PlayerScore = ShooterPlayerState ? ShooterPlayerState->GetScore() : 0;
			
			// Fire session end event for all cases
			FOnlineEventParms Params;
			Params.Add( TEXT( "GameplayModeId" ), FVariantData( (int32)1 ) ); // @todo determine game mode (ffa v tdm)
			Params.Add( TEXT( "DifficultyLevelId" ), FVariantData( (int32)0 ) ); // unused
			Params.Add( TEXT( "ExitStatusId" ), FVariantData( (int32)0 ) ); // unused
			Params.Add( TEXT( "PlayerScore" ), FVariantData( (int32)PlayerScore ) );
			Params.Add( TEXT( "PlayerWon" ), FVariantData( (bool)bIsWinner ) );
			Params.Add( TEXT( "MapName" ), FVariantData( MapName ) );
			Params.Add( TEXT( "MapNameString" ), FVariantData( MapName ) ); // @todo workaround for a bug in backend service, remove when fixed
			
			Events->TriggerEvent(*UniqueId, TEXT("PlayerSessionEnd"), Params);

			// Online matches require the MultiplayerRoundEnd event as well
			//UCoreGameInstance* SGI = GetWorld() != NULL ? Cast<UCoreGameInstance>(GetWorld()->GetGameInstance()) : NULL;
			//if (SGI->GetIsOnline())
			//{
			//	FOnlineEventParms MultiplayerParams;

			//	ACoreGameState* const MyGameState = GetWorld() != NULL ? GetWorld()->GetGameState<ACoreGameState>() : NULL;
			//	if (ensure(MyGameState != nullptr))
			//	{
			//		MultiplayerParams.Add( TEXT( "SectionId" ), FVariantData( (int32)0 ) ); // unused
			//		MultiplayerParams.Add( TEXT( "GameplayModeId" ), FVariantData( (int32)1 ) ); // @todo determine game mode (ffa v tdm)
			//		MultiplayerParams.Add( TEXT( "MatchTypeId" ), FVariantData( (int32)1 ) ); // @todo abstract the specific meaning of this value across platforms
			//		MultiplayerParams.Add( TEXT( "DifficultyLevelId" ), FVariantData( (int32)0 ) ); // unused
			//		MultiplayerParams.Add( TEXT( "TimeInSeconds" ), FVariantData( (float)ExpendedTimeInSeconds ) );
			//		MultiplayerParams.Add( TEXT( "ExitStatusId" ), FVariantData( (int32)0 ) ); // unused
			//		
			//		Events->TriggerEvent(*UniqueId, TEXT("MultiplayerRoundEnd"), MultiplayerParams);
			//	}
			//}
		}

		bHasSentStartEvents = false;
	}
}

void AGamePlayerController::SetCinematicMode(bool bInCinematicMode, bool bHidePlayer, bool bAffectsHUD, bool bAffectsMovement, bool bAffectsTurning)
{
	Super::SetCinematicMode(bInCinematicMode, bHidePlayer, bAffectsHUD, bAffectsMovement, bAffectsTurning);

	// If we have a pawn we need to determine if we should show/hide the weapon
	ACharacterBase* MyPawn = Cast<ACharacterBase>(GetPawn());
	AWeapon* MyWeapon = MyPawn ? MyPawn->GetWeapon() : NULL;
	if (MyWeapon)
	{
		if (bInCinematicMode && bHidePlayer)
		{
			MyWeapon->SetActorHiddenInGame(true);
		}
		else if (!bCinematicMode)
		{
			MyWeapon->SetActorHiddenInGame(false);
		}
	}
}

bool AGamePlayerController::IsMoveInputIgnored() const
{
	if (IsInState(NAME_Spectating))
	{
		return false;
	}
	else
	{
		return Super::IsMoveInputIgnored();
	}
}

bool AGamePlayerController::IsLookInputIgnored() const
{
	if (IsInState(NAME_Spectating))
	{
		return false;
	}
	else
	{
		return Super::IsLookInputIgnored();
	}
}

void AGamePlayerController::InitInputSystem()
{
	Super::InitInputSystem();

	auto PersistentUser = GetPersistentUser();
	if(PersistentUser)
	{
		PersistentUser->TellInputAboutKeybindings();
	}
}

void AGamePlayerController::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME_CONDITION( AGamePlayerController, bInfiniteAmmo, COND_OwnerOnly );
	DOREPLIFETIME_CONDITION( AGamePlayerController, bInfiniteClip, COND_OwnerOnly );
}

void AGamePlayerController::Suicide()
{
	if ( IsInState(NAME_Playing) )
	{
		ServerSuicide();
	}
}

bool AGamePlayerController::ServerSuicide_Validate()
{
	return true;
}

void AGamePlayerController::ServerSuicide_Implementation()
{
	if ( (GetPawn() != nullptr) && ((GetWorld()->TimeSeconds - GetPawn()->CreationTime > 1) || (GetNetMode() == NM_Standalone)) )
	{
		ACharacterBase* MyPawn = Cast<ACharacterBase>(GetPawn());
		if (MyPawn)
		{
			MyPawn->Suicide();
		}
	}
}

bool AGamePlayerController::HasInfiniteAmmo() const
{
	return bInfiniteAmmo;
}

bool AGamePlayerController::HasInfiniteClip() const
{
	return bInfiniteClip;
}

bool AGamePlayerController::HasHealthRegen() const
{
	return bHealthRegen;
}

bool AGamePlayerController::HasGodMode() const
{
	return bGodMode;
}

bool AGamePlayerController::IsGameInputAllowed() const
{
	return bAllowGameActions && !bCinematicMode;
}

void AGamePlayerController::ToggleChatWindow()
{
	// TODO
	//AShooterHUD* ShooterHUD = Cast<AShooterHUD>(GetHUD());
	//if (ShooterHUD)
	//{
	//	ShooterHUD->ToggleChat();
	//}
}

void AGamePlayerController::ClientTeamMessage_Implementation( APlayerState* SenderPlayerState, const FString& S, FName Type, float MsgLifeTime  )
{
	// TODO
	//AShooterHUD* ShooterHUD = Cast<AShooterHUD>(GetHUD());
	//if (ShooterHUD)
	//{
	//	if( Type == ServerSayString )
	//	{
	//		if( SenderPlayerState != PlayerState  )
	//		{
	//			ShooterHUD->AddChatLine(FText::FromString(S), false);
	//		}
	//	}
	//}
}

void AGamePlayerController::Say( const FString& Msg )
{
	ServerSay(Msg.Left(128));
}

bool AGamePlayerController::ServerSay_Validate( const FString& Msg )
{
	return true;
}

void AGamePlayerController::ServerSay_Implementation( const FString& Msg )
{
	GetWorld()->GetAuthGameMode<ACoreGameModeBase>()->Broadcast(this, Msg, ServerSayString);
}

// TODO
//AShooterHUD* AGamePlayerController::GetShooterHUD() const
//{
//	return Cast<AShooterHUD>(GetHUD());
//}


UPersistentUser* AGamePlayerController::GetPersistentUser() const
{
	UGameLocalPlayer* const LocalPlayer = Cast<UGameLocalPlayer>(Player);
	return LocalPlayer ? LocalPlayer->GetPersistentUser() : nullptr;
}

bool AGamePlayerController::SetPause(bool bPause, FCanUnpause CanUnpauseDelegate /*= FCanUnpause()*/)
{
	const bool Result = APlayerController::SetPause(bPause, CanUnpauseDelegate);

	// Update rich presence.
	const auto PresenceInterface = Online::GetPresenceInterface();
	const auto Events = Online::GetEventsInterface();
	const auto LocalPlayer = Cast<ULocalPlayer>(Player);
	TSharedPtr<const FUniqueNetId> UserId = LocalPlayer ? LocalPlayer->GetCachedUniqueNetId() : nullptr;

	if(PresenceInterface.IsValid() && UserId.IsValid())
	{
		FOnlineUserPresenceStatus PresenceStatus;
		if(Result && bPause)
		{
			PresenceStatus.Properties.Add(DefaultPresenceKey, FString("Paused"));
		}
		else
		{
			PresenceStatus.Properties.Add(DefaultPresenceKey, FString("InGame"));
		}
		PresenceInterface->SetPresence(*UserId, PresenceStatus);

	}

	// Don't send pause events while online since the game doesn't actually pause
	if(GetNetMode() == NM_Standalone && Events.IsValid() && PlayerState->UniqueId.IsValid())
	{
		FOnlineEventParms Params;
		Params.Add( TEXT( "GameplayModeId" ), FVariantData( (int32)1 ) ); // @todo determine game mode (ffa v tdm)
		Params.Add( TEXT( "DifficultyLevelId" ), FVariantData( (int32)0 ) ); // unused
		if(Result && bPause)
		{
			Events->TriggerEvent(*PlayerState->UniqueId, TEXT("PlayerSessionPause"), Params);
		}
		else
		{
			Events->TriggerEvent(*PlayerState->UniqueId, TEXT("PlayerSessionResume"), Params);
		}
	}

	return Result;
}

void AGamePlayerController::ShowInGameMenu()
{
	// TODO
	//AShooterHUD* ShooterHUD = GetShooterHUD();	
	//if(ShooterIngameMenu.IsValid() && !ShooterIngameMenu->GetIsGameMenuUp() && ShooterHUD && (ShooterHUD->IsMatchOver() == false))
	//{
	//	ShooterIngameMenu->ToggleGameMenu();
	//}
}
void AGamePlayerController::UpdateAchievementsOnGameEnd()
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	if (LocalPlayer)
	{
		auto ShooterPlayerState = Cast<AGamePlayerState>(PlayerState);
		if (ShooterPlayerState)
		{			
			auto  PersistentUser = GetPersistentUser();

			if (PersistentUser)
			{						
				const int32 Wins = PersistentUser->GetWins();
				const int32 Losses = PersistentUser->GetLosses();
				const int32 Matches = Wins + Losses;

				const int32 TotalKills = PersistentUser->GetKills();
				const int32 MatchScore = (int32)ShooterPlayerState->GetScore();

				const int32 TotalBulletsFired = PersistentUser->GetBulletsFired();
				const int32 TotalRocketsFired = PersistentUser->GetRocketsFired();
			
				float TotalGameAchievement = 0;
				float CurrentGameAchievement = 0;
			
				///////////////////////////////////////
				// Kill achievements
				if (TotalKills >= 1)
				{
					CurrentGameAchievement += 100.0f;
				}
				TotalGameAchievement += 100;

				{
					float fSomeKillPct = ((float)TotalKills / (float)SomeKillsCount) * 100.0f;
					fSomeKillPct = FMath::RoundToFloat(fSomeKillPct);
					UpdateAchievementProgress(ACH_SOME_KILLS, fSomeKillPct);

					CurrentGameAchievement += FMath::Min(fSomeKillPct, 100.0f);
					TotalGameAchievement += 100;
				}

				{
					float fLotsKillPct = ((float)TotalKills / (float)LotsKillsCount) * 100.0f;
					fLotsKillPct = FMath::RoundToFloat(fLotsKillPct);
					UpdateAchievementProgress(ACH_LOTS_KILLS, fLotsKillPct);

					CurrentGameAchievement += FMath::Min(fLotsKillPct, 100.0f);
					TotalGameAchievement += 100;
				}
				///////////////////////////////////////

				///////////////////////////////////////
				// Match Achievements
				{
					UpdateAchievementProgress(ACH_FINISH_MATCH, 100.0f);

					CurrentGameAchievement += 100;
					TotalGameAchievement += 100;
				}
			
				{
					float fLotsRoundsPct = ((float)Matches / (float)LotsMatchesCount) * 100.0f;
					fLotsRoundsPct = FMath::RoundToFloat(fLotsRoundsPct);
					UpdateAchievementProgress(ACH_LOTS_MATCHES, fLotsRoundsPct);

					CurrentGameAchievement += FMath::Min(fLotsRoundsPct, 100.0f);
					TotalGameAchievement += 100;
				}
				///////////////////////////////////////

				///////////////////////////////////////
				// Win Achievements
				if (Wins >= 1)
				{
					UpdateAchievementProgress(ACH_FIRST_WIN, 100.0f);

					CurrentGameAchievement += 100.0f;
				}
				TotalGameAchievement += 100;

				{			
					float fLotsWinPct = ((float)Wins / (float)LotsWinsCount) * 100.0f;
					fLotsWinPct = FMath::RoundToInt(fLotsWinPct);
					UpdateAchievementProgress(ACH_LOTS_WIN, fLotsWinPct);

					CurrentGameAchievement += FMath::Min(fLotsWinPct, 100.0f);
					TotalGameAchievement += 100;
				}

				{			
					float fManyWinPct = ((float)Wins / (float)ManyWinsCount) * 100.0f;
					fManyWinPct = FMath::RoundToInt(fManyWinPct);
					UpdateAchievementProgress(ACH_MANY_WIN, fManyWinPct);

					CurrentGameAchievement += FMath::Min(fManyWinPct, 100.0f);
					TotalGameAchievement += 100;
				}
				///////////////////////////////////////

				///////////////////////////////////////
				// Ammo Achievements
				{
					float fLotsBulletsPct = ((float)TotalBulletsFired / (float)LotsBulletsCount) * 100.0f;
					fLotsBulletsPct = FMath::RoundToFloat(fLotsBulletsPct);
					UpdateAchievementProgress(ACH_SHOOT_BULLETS, fLotsBulletsPct);

					CurrentGameAchievement += FMath::Min(fLotsBulletsPct, 100.0f);
					TotalGameAchievement += 100;
				}

				{
					float fLotsRocketsPct = ((float)TotalRocketsFired / (float)LotsRocketsCount) * 100.0f;
					fLotsRocketsPct = FMath::RoundToFloat(fLotsRocketsPct);
					UpdateAchievementProgress(ACH_SHOOT_ROCKETS, fLotsRocketsPct);

					CurrentGameAchievement += FMath::Min(fLotsRocketsPct, 100.0f);
					TotalGameAchievement += 100;
				}
				///////////////////////////////////////

				///////////////////////////////////////
				// Score Achievements
				{
					float fGoodScorePct = ((float)MatchScore / (float)GoodScoreCount) * 100.0f;
					fGoodScorePct = FMath::RoundToFloat(fGoodScorePct);
					UpdateAchievementProgress(ACH_GOOD_SCORE, fGoodScorePct);
				}

				{
					float fGreatScorePct = ((float)MatchScore / (float)GreatScoreCount) * 100.0f;
					fGreatScorePct = FMath::RoundToFloat(fGreatScorePct);
					UpdateAchievementProgress(ACH_GREAT_SCORE, fGreatScorePct);
				}
				///////////////////////////////////////

				///////////////////////////////////////
				// Map Play Achievements
				UWorld* World = GetWorld();
				if (World)
				{			
					FString MapName = *FPackageName::GetShortName(World->PersistentLevel->GetOutermost()->GetName());
					if (MapName.Find(TEXT("Highrise")) != -1)
					{
						UpdateAchievementProgress(ACH_PLAY_HIGHRISE, 100.0f);
					}
					else if (MapName.Find(TEXT("Sanctuary")) != -1)
					{
						UpdateAchievementProgress(ACH_PLAY_SANCTUARY, 100.0f);
					}
				}
				///////////////////////////////////////			

				const auto Events = Online::GetEventsInterface();
				const auto Identity = Online::GetIdentityInterface();

				if (Events.IsValid() && Identity.IsValid())
				{							
					const int32 UserIndex = LocalPlayer->GetControllerId();
					TSharedPtr<const FUniqueNetId> UniqueID = Identity->GetUniquePlayerId(UserIndex);
					if (UniqueID.IsValid())
					{				
						FOnlineEventParms Params;

						float fGamePct = (CurrentGameAchievement / TotalGameAchievement) * 100.0f;
						fGamePct = FMath::RoundToFloat(fGamePct);
						Params.Add( TEXT( "CompletionPercent" ), FVariantData( (float)fGamePct ) );
						if (UniqueID.IsValid())
						{				
							Events->TriggerEvent(*UniqueID, TEXT("GameProgress"), Params);
						}
					}
				}
			}
		}
	}
}

void AGamePlayerController::UpdateLeaderboardsOnGameEnd()
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	if (LocalPlayer)
	{
		// update leaderboards - note this does not respect existing scores and overwrites them. We would first need to read the leaderboards if we wanted to do that.
		IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
		if (OnlineSub)
		{
			IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
			if (Identity.IsValid())
			{
				TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(LocalPlayer->GetControllerId());
				if (UserId.IsValid())
				{
					IOnlineLeaderboardsPtr Leaderboards = OnlineSub->GetLeaderboardsInterface();
					if (Leaderboards.IsValid())
					{
						auto GamePlayerState = Cast<AGamePlayerState>(PlayerState);
						if (GamePlayerState)
						{
							FAllTimeMatchResultsWrite ResultsWriteObject;

							ResultsWriteObject.SetIntStat(LEADERBOARD_STAT_SCORE, GamePlayerState->GetKills());
							ResultsWriteObject.SetIntStat(LEADERBOARD_STAT_KILLS, GamePlayerState->GetKills());
							ResultsWriteObject.SetIntStat(LEADERBOARD_STAT_DEATHS, GamePlayerState->GetDeaths());
							ResultsWriteObject.SetIntStat(LEADERBOARD_STAT_MATCHESPLAYED, 1);

							// the call will copy the user id and write object to its own memory
							Leaderboards->WriteLeaderboards(GamePlayerState->SessionName, *UserId, ResultsWriteObject);
							Leaderboards->FlushLeaderboards(TEXT("SURVIVAL"));
						}
					}
				}
			}
		}
	}
}

void AGamePlayerController::UpdateSaveFileOnGameEnd(bool bIsWinner)
{
	auto GamePlayerState = Cast<AGamePlayerState>(PlayerState);
	if (GamePlayerState)
	{
		// update local saved profile
		auto PersistentUser = GetPersistentUser();
		if (PersistentUser)
		{
			PersistentUser->AddMatchResult(GamePlayerState->GetKills(), GamePlayerState->GetDeaths(), GamePlayerState->GetNumBulletsFired(), GamePlayerState->GetNumRocketsFired(), bIsWinner);
			PersistentUser->SaveIfDirty();
		}
	}
}

void AGamePlayerController::PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel)
{
	Super::PreClientTravel( PendingURL, TravelType, bIsSeamlessTravel );

	if ( GetWorld() != nullptr )
	{
		UViewportClient* ShooterViewport = Cast<UViewportClient>( GetWorld()->GetGameViewport() );

		if ( ShooterViewport != nullptr)
		{
			ShooterViewport->ShowLoadingScreen();
		}
		
		// TODO
		//AShooterHUD* ShooterHUD = Cast<AShooterHUD>(GetHUD());
		//if (ShooterHUD != nullptr)
		//{
		//	// Passing true to bFocus here ensures that focus is returned to the game viewport.
		//	ShooterHUD->ShowScoreboard(false, true);
		//}
	}
}
