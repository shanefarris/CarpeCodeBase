
#pragma once

#include "Online/CoreGameModeBase.h"
#include "CoreGameModeStandard.generated.h"

class AGamePlayerState;
class AAIController;

UCLASS()
class ACoreGameModeStandard : public ACoreGameModeBase
{
	GENERATED_UCLASS_BODY()

public:
	/** setup team changes at player login */
	void PostLogin(APlayerController* NewPlayer) override;

	/** initialize replicated game data */
	virtual void InitGameState() override;

	/** can players damage each other? */
	virtual bool CanDealDamage(AGamePlayerState* DamageInstigator, AGamePlayerState* DamagedPlayer) const /*override*/;

protected:

	/** number of teams */
	int32 NumTeams;

	/** pick team with least players in or random when it's equal */
	int32 ChooseTeam(AGamePlayerState* ForPlayerState) const;

	/** check team constraints */
	virtual bool IsSpawnpointAllowed(APlayerStart* SpawnPoint, AController* Player) const;

	/** initialization for bot after spawning */
	virtual void InitBot(AAIController* AIC, int32 BotNum) override;
};