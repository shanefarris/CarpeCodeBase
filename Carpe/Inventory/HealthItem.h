// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "InventoryItem.h"
#include "HealthItem.generated.h"

class ACharacterBase;

// A pickup object that replenishes character health
UCLASS(Blueprintable)
class AHealthItem : public AInventoryItem
{
	GENERATED_UCLASS_BODY()

public:
	/** check if pawn can use this pickup */
	virtual bool CanBePickedUp(ACharacterBase* TestPawn) const override;

protected:

	/** how much health does it give? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG")
	int32 Health;

	/** give pickup */
	virtual void GivePickupTo(ACharacterBase* Pawn) override;
};
