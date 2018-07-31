// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Runtime/Engine/Classes/Engine/Canvas.h"
#include "GameDamageType.generated.h"

// DamageType class that specifies an icon to display
UCLASS(const, Blueprintable, BlueprintType)
class UGameDamageType : public UDamageType
{
	GENERATED_UCLASS_BODY()

public:

	bool  GetCanDieFrom();
	float GetHeadDamageModifier();
	float GetLimbDamageModifier();

	void SetCanDieFrom(bool Value);
	void SetHeadDamageModifier(float Value);
	void SetLimbDamageModifier(float Value);

private:
	/** icon displayed in death messages log when killed with this weapon */
	UPROPERTY(EditAnywhere, Category = "CFG|CustomDamage")
	FCanvasIcon KillIcon;

	/** force feedback effect to play on a player hit by this damage type */
	UPROPERTY(EditAnywhere, Category = "CFG|CustomDamage")
	UForceFeedbackEffect *HitForceFeedback;

	/** force feedback effect to play on a player killed by this damage type */
	UPROPERTY(EditAnywhere, Category = "CFG|CustomDamage")
	UForceFeedbackEffect *KilledForceFeedback;

	/* Can player die from this damage type (eg. players don't die from hunger) */
	UPROPERTY(EditAnywhere, Category = "CFG|CustomDamage")
	bool bCanDieFrom;

	/* Damage modifier for headshot damage */
	UPROPERTY(EditAnywhere, Category = "CFG|CustomDamage")
	float HeadDmgModifier;

	UPROPERTY(EditAnywhere, Category = "CFG|CustomDamage")
	float LimbDmgModifier;
};



