#include "MenuGameMode.h"
#include "Carpe.h"
//#include "ShooterMainMenu.h"
//#include "ShooterWelcomeMenu.h"
//#include "ShooterMessageMenu.h"
#include "Player/PlayerController_Menu.h"
#include "Online/Session.h"

AMenuGameMode::AMenuGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerControllerClass = APlayerController_Menu::StaticClass();
}

void AMenuGameMode::RestartPlayer(class AController* NewPlayer)
{
	// don't restart
}

/** Returns game session class to use */
TSubclassOf<AGameSession> AMenuGameMode::GetGameSessionClass() const
{
	return ASession::StaticClass();
}
