#include "Player/Spectator.h"
#include "Carpe.h"
//#include "UI/Menu/ShooterDemoPlaybackMenu.h"
//#include "UI/Widgets/SShooterDemoHUD.h"
#include "Engine/DemoNetDriver.h"

ASpectator::ASpectator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	bShouldPerformFullTickWhenPaused = true;
}

void ASpectator::SetupInputComponent()
{
	Super::SetupInputComponent();

	// UI input
	InputComponent->BindAction( "InGameMenu", IE_Pressed, this, &ASpectator::OnToggleInGameMenu );

	InputComponent->BindAction( "NextWeapon", IE_Pressed, this, &ASpectator::OnIncreasePlaybackSpeed );
	InputComponent->BindAction( "PrevWeapon", IE_Pressed, this, &ASpectator::OnDecreasePlaybackSpeed );
}

void ASpectator::SetPlayer( UPlayer* InPlayer )
{
	Super::SetPlayer( InPlayer );

	// TODO
	// Build menu only after game is initialized
	//ShooterDemoPlaybackMenu = MakeShareable( new FShooterDemoPlaybackMenu() );
	//ShooterDemoPlaybackMenu->Construct( Cast< ULocalPlayer >( Player ) );

	// Create HUD if this is playback
	if (GetWorld() != nullptr && GetWorld()->DemoNetDriver != nullptr && !GetWorld()->DemoNetDriver->IsServer())
	{
		if (GEngine != nullptr && GEngine->GameViewport != nullptr)
		{
			// TODO
			//DemoHUD = SNew(SShooterDemoHUD).PlayerOwner(this);
			//GEngine->GameViewport->AddViewportWidgetContent(DemoHUD.ToSharedRef());
		}
	}

	FActorSpawnParameters SpawnInfo;

	SpawnInfo.Owner				= this;
	SpawnInfo.Instigator		= Instigator;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PlaybackSpeed = 2;

	FInputModeGameAndUI InputMode;
	// TODO InputMode.SetWidgetToFocus(DemoHUD);

	SetInputMode(InputMode);
}

void ASpectator::OnToggleInGameMenu()
{
	// TODO
	// if no one's paused, pause
	//if ( ShooterDemoPlaybackMenu.IsValid() )
	//{
	//	ShooterDemoPlaybackMenu->ToggleGameMenu();
	//}
}

static float PlaybackSpeedLUT[5] = { 0.1f, 0.5f, 1.0f, 2.0f, 4.0f };

void ASpectator::OnIncreasePlaybackSpeed()
{
	PlaybackSpeed = FMath::Clamp( PlaybackSpeed + 1, 0, 4 );

	GetWorldSettings()->DemoPlayTimeDilation = PlaybackSpeedLUT[ PlaybackSpeed ];
}

void ASpectator::OnDecreasePlaybackSpeed()
{
	PlaybackSpeed = FMath::Clamp( PlaybackSpeed - 1, 0, 4 );

	GetWorldSettings()->DemoPlayTimeDilation = PlaybackSpeedLUT[ PlaybackSpeed ];
}

void ASpectator::Destroyed()
{
	// TODO 
	//if (GEngine != nullptr && GEngine->GameViewport != nullptr && DemoHUD.IsValid())
	//{
	//	// Remove HUD
	//	GEngine->GameViewport->RemoveViewportWidgetContent(DemoHUD.ToSharedRef());
	//}

	Super::Destroyed();
}
