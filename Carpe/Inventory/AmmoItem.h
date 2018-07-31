// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "InventoryItem.h"
#include "AmmoItem.generated.h"

class ACharacterBase;
class AWeapon;

// A pickup object that replenishes ammunition for a weapon
UCLASS(Blueprintable)
class AAmmoItem : public AInventoryItem
{
	GENERATED_UCLASS_BODY()

public:
	/** check if pawn can use this pickup */
	virtual bool CanBePickedUp(ACharacterBase* TestPawn) const override;

	bool IsForWeapon(UClass* WeaponClass);

protected:

	/** how much ammo does it give? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CFG")
	int32 AmmoClips;

	/** which weapon gets ammo? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG")
	TSubclassOf<AWeapon> WeaponType;

	/** give pickup */
	virtual void GivePickupTo(ACharacterBase* Pawn) override;
};
