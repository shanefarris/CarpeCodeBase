
#pragma once

#include "CapturableResourceBase.generated.h"

// Base class for resources in a world that can be catured, like a flag pole going up or down showing that land has been captured.
UCLASS(abstract)
class ACapturableResourceBase : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	/** initial setup */
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = Capture)
	virtual void StartCapture();

	UFUNCTION(BlueprintCallable, Category = Capture)
	virtual void UpdateCapture();

	UFUNCTION(BlueprintCallable, Category = Capture)
	virtual void EndCapture();

protected:

	FTimerHandle TimerHandle_Capturing;

	
};
