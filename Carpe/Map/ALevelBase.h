#pragma once

#include "Runtime/Engine/Classes/Engine/LevelScriptActor.h"
#include "ALevelBase.generated.h"

UCLASS(abstract)
class ALevelBase : public ALevelScriptActor
{
	GENERATED_UCLASS_BODY()

public:
	/** This triggers a save / update of all the Solus Save Game level data. This should be called before the level is unloaded/unstreamed via blueprints.*/
	UFUNCTION(BlueprintCallable, Category = "Core Level")
	void UpdateLevelSaveData();

protected:

	/** A Custom name for each Level, specified in Level BP Default Properties! */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Core Level")
	FName SolusLevelName;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;

};