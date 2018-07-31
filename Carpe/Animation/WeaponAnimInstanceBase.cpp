#include "WeaponAnimInstanceBase.h"
#include "Carpe.h"
#include "Player/CharacterBase.h"
#include "Inventory/Weapon.h"

void UWeaponAnimInstanceBase::NativeInitializeAnimation()
{
	//Very Important Line
	Super::NativeInitializeAnimation();

	//Cache the owning pawn for use in Tick
	
	Character = nullptr;
	Weapon = nullptr;

	bIsReloading = false;
	bIsFiring = false;
	bIsShotFired = false;
	ReloadSpeed = 1.0f;

	UpdateState();
}

// Event Blueprint Update Animation
void UWeaponAnimInstanceBase::NativeUpdateAnimation(float DeltaTimeX)
{
	//Very Important Line
	Super::NativeUpdateAnimation(DeltaTimeX);

	if (Character == nullptr)
	{
		UpdateState();
	}

	if (Character)
	{
		if (Character->bIsDying)
		{
			bIsReloading = false;
			bIsFiring = false;
			return;
		}

		bIsReloading = Character->bIsReloading;
		bIsFiring = Character->IsFiring();
		ReloadSpeed = Character->ReloadSpeed;
	}
}

void UWeaponAnimInstanceBase::UpdateState()
{
	if (Character == nullptr)
	{
		auto _character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (_character)
		{
			Character = Cast<ACharacterBase>(_character);
			Weapon = Character->GetWeapon();
		}
	}
}