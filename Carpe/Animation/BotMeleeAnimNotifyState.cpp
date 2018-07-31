#include "BotMeleeAnimNotifyState.h"
#include "Carpe.h"
#include "AI/Bot.h"


void UBotMeleeAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) 
{ 
	// Spawn the sound 
	if (SoundToPlay) 
	{ 
		AudioComponent = UGameplayStatics::SpawnSoundAttached(SoundToPlay, MeshComp, NAME_None, FVector(ForceInit), EAttachLocation::KeepRelativeOffset, true); 
	}

	if (MeshComp)
	{
		auto pawn = Cast<ABot>(MeshComp->GetOwner());
		if (pawn != nullptr)
		{
			pawn->PerformMeleeStrike(nullptr);
		}
	}
}

void UBotMeleeAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{ 
	// Stop the sound if needed 
	if (AudioComponent) 
	{ 
		AudioComponent->Stop(); 
		AudioComponent = nullptr; 
	} 
}