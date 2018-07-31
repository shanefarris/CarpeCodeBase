// Copyright 2017 Corn Fed Games LLC

#pragma once

#include "Blueprint/UserWidget.h"
#include "Templates/SharedPointer.h"
#include "CoreHudComponent.generated.h"

struct FCategoryEntity;

UCLASS()
class UCoreHudComponent : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (DisplayName = "HudUpdate"), Category = "HUD")
	bool HudUpdate();

};

