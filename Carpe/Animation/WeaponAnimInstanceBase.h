#pragma once

#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "WeaponAnimInstanceBase.generated.h"

class ACharacterBase;
class AWeapon;

UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class UWeaponAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()
public:
	ACharacterBase* Character;
	AWeapon* Weapon;
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	bool bIsReloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	bool bIsFiring;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	float ReloadSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	bool bIsShotFired;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

private:
	void UpdateState();

};