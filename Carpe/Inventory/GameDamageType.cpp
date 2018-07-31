#include "Inventory/GameDamageType.h"
#include "Carpe.h"

UGameDamageType::UGameDamageType(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	/* We apply this modifier based on the physics material setup to the head of the enemy PhysAsset */
	HeadDmgModifier = 2.0f;
	LimbDmgModifier = 0.5f;
	bCanDieFrom = true;
}


bool UGameDamageType::GetCanDieFrom()
{
	return bCanDieFrom;
}


float UGameDamageType::GetHeadDamageModifier()
{
	return HeadDmgModifier;
}

float UGameDamageType::GetLimbDamageModifier()
{
	return LimbDmgModifier;
}

void UGameDamageType::SetCanDieFrom(bool Value)
{
	bCanDieFrom = Value;
}

void UGameDamageType::SetHeadDamageModifier(float Value)
{
	HeadDmgModifier = Value;
}

void UGameDamageType::SetLimbDamageModifier(float Value)
{
	LimbDmgModifier = Value;
}