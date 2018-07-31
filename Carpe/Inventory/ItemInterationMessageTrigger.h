// Copyright 2015 Corn Fed Games LLC

#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/PointLightComponent.h"
#include "ItemInterationMessageTrigger.generated.h"

class UBoxComponent;

/**
 *  Used to message the player once they walk up to an in-game item.  E.g "Press 'F' to pickup".
 */
UCLASS(Blueprintable)
class AItemInterationMessageTrigger : public AActor
{
	GENERATED_UCLASS_BODY()

private:
	UBoxComponent* Box;

#if defined(NDEBUG)
	// Debug light to show we are in or out of the trigger box.
	UPointLightComponent* Light;
#endif

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Options")
	bool bShowDebugLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Options")
	FString InteractiveMessage;

	UFUNCTION()
	void Debug(FString message);

	UFUNCTION()
	void TriggerEnter(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void TriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
