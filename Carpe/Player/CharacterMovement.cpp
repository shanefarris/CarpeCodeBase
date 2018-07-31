#include "Player/CharacterMovement.h"
#include "Carpe.h"

//----------------------------------------------------------------------//
// UPawnMovementComponent
//----------------------------------------------------------------------//
UCharacterMovement::UCharacterMovement(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


float UCharacterMovement::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	const ACharacterBase* Owner = Cast<ACharacterBase>(PawnOwner);
	if (Owner)
	{
		if (Owner->IsTargeting())
		{
			MaxSpeed *= Owner->GetTargetingSpeedModifier();
		}
		if (Owner->IsRunning())
		{
			MaxSpeed *= Owner->GetRunningSpeedModifier();
		}
	}

	return MaxSpeed;
}
