
#pragma once

#include "Runtime/Engine/Classes/Animation/AnimNotifies/AnimNotifyState.h"
#include "BotMeleeAnimNotifyState.generated.h"

class USoundBase;
class UAudioComponent;
class USkeletalMeshComponent;
class UAnimSequenceBase;

UCLASS()
class UBotMeleeAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

	// The animation is starting
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	// The animation stoped
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

 public:
	// Sound to Play
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	USoundBase* SoundToPlay;

	// The audio player
	UPROPERTY(BlueprintReadWrite, Category = "CFG")
	UAudioComponent* AudioComponent;
 };


