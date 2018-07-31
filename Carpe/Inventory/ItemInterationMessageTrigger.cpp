#include "ItemInterationMessageTrigger.h"
#include "Carpe.h"

#include "OnlineDelegateMacros.h"
#include "VoiceInterface.h"

#include "Inventory/GameDamageType.h"
#include "Player/CharacterBase.h"
#include "UI/CoreHudBase.h"

AItemInterationMessageTrigger::AItemInterationMessageTrigger(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Box = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Box"));
	Box->bGenerateOverlapEvents = true;
	Box->SetRelativeScale3D(FVector(1, 1, 1));
	RootComponent = Box;
	bShowDebugLight = true;
	InteractiveMessage = "Press 'E' to pickup weapon";

#if defined(NDEBUG)
	Light = ObjectInitializer.CreateDefaultSubobject<UPointLightComponent>(this, TEXT("Light"));
	Light->Intensity = 10,000;
	Light->SetLightColor(FLinearColor::Red);
	Light->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
#endif

	Box->OnComponentBeginOverlap.AddDynamic(this, &AItemInterationMessageTrigger::TriggerEnter);
	Box->OnComponentEndOverlap.AddDynamic(this, &AItemInterationMessageTrigger::TriggerExit);
}

void AItemInterationMessageTrigger::Debug(FString message)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, message);
	}
}

void AItemInterationMessageTrigger::TriggerEnter(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult & SweepResult)
{
	auto controller = GetWorld()->GetFirstPlayerController();
	auto hud = controller ? Cast<ACoreHudBase>(controller->GetHUD()) : nullptr;
	if (hud)
	{
		hud->SetMessageText(InteractiveMessage);
		hud->SetMessageVisible(true);
	}

#if defined(NDEBUG)
	if (bShowDebugLight)
	{
		Light->SetLightColor(FLinearColor::Green);
		Debug(TEXT("Enter"));
	}
#endif
}

void AItemInterationMessageTrigger::TriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto controller = GetWorld()->GetFirstPlayerController();
	auto hud = controller ? Cast<ACoreHudBase>(controller->GetHUD()) : nullptr;
	if (hud)
	{
		hud->SetMessageVisible(false);
	}

#if defined(NDEBUG)
	if (bShowDebugLight)
	{
		Light->SetLightColor(FLinearColor::Red);
		Debug(TEXT("Exit"));
	}
#endif
}
