#include "CoreSaveGame.h"
#include "Carpe.h"

UCoreSaveGame::UCoreSaveGame()
{
	SaveSlotName = TEXT("TestSaveSlot");
	UserIndex = 0;
}

void UCoreSaveGame::SaveGame()
{
	auto PlayerName = TEXT("PlayerOne");
	auto SaveGameInstance = Cast<UCoreSaveGame>(UGameplayStatics::CreateSaveGameObject(UCoreSaveGame::StaticClass()));
	SaveGameInstance->PlayerName = PlayerName;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
}

void UCoreSaveGame::LoadGame()
{
	auto LoadGameInstance = Cast<UCoreSaveGame>(UGameplayStatics::CreateSaveGameObject(UCoreSaveGame::StaticClass()));
	LoadGameInstance = Cast<UCoreSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
	auto PlayerNameToDisplay = LoadGameInstance->PlayerName;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, PlayerNameToDisplay);
	}
}