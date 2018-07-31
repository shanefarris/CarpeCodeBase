#include "AnimInstanceBase.h"
#include "Carpe.h"

void UAnimInstanceBase::NativeInitializeAnimation()
{
	//Very Important Line
	Super::NativeInitializeAnimation();

	//Cache the owning pawn for use in Tick
	OwningPawn = TryGetPawnOwner();

	Speed = 0.0f;
	Direction = 0.0f;
	IsTargeting = false;
}

// Event Blueprint Update Animation
void UAnimInstanceBase::NativeUpdateAnimation(float DeltaTimeX)
{
	//Very Important Line
	Super::NativeUpdateAnimation(DeltaTimeX);

	//Always Check Pointers
	if (!OwningPawn)
	{
		return;
	}

	auto Character = Cast<ACharacterBase>(OwningPawn);

	if (Character->bIsDying)
	{
		return;
	}

	Speed = Character->GetVelocity().Size();

	// Direction
	if (Speed > 0.0)
	{
		auto Rotation = Character->GetActorRotation();
		auto B = Rotation.GetInverse();
		auto A = Character->GetVelocity().Rotation();

		auto AQuat = FQuat(A);
		auto BQuat = FQuat(B);
		auto CombinedRotation = FRotator(BQuat * AQuat);
		if (CombinedRotation.Yaw >= 180)
		{
			Direction -= 360;
		}
		else
		{
			Direction = CombinedRotation.Yaw;
		}
	}
	else
	{
		Direction = 0.0f;
	}

	if (Character->CurrentWeapon)
	{
		CurrentInventoryItemLocomotion = Character->CurrentWeapon->PlayerLocomotionAnim;
	}

	// Firing
	IsFiring = Character->IsFiring();
	if (IsFiring && Character->CurrentWeapon)
	{
		CurrentInventoryItemUse = Character->CurrentWeapon->PlayerUseAnim;
	}

	// Reloading
	if (Character->bIsReloading && Character->CurrentWeapon)
	{
		CurrentInventoryItemReload = Character->CurrentWeapon->PlayerReloadAnim;
		IsReloading = true;
	}
	else
	{
		IsReloading = false;
	}

	// Health
	Health = Character->Health;

	// Targeting
	IsTargeting = Character->IsTargeting();

	// Aim pitch and yaw
	auto AimOffset = Character->GetAimOffsets();
	AimPitch = AimOffset.Pitch / 180;
	AimYaw = AimOffset.Yaw / 180;

	// Running
	IsRunning = Character->IsRunning();

	// Jumping
	IsJumping = Character->GetMovementComponent()->Velocity.Z > 1.0f;

	// Jump time
	if (IsJumping)
	{
		JumpTime += DeltaTimeX;
	}
	else
	{
		JumpTime = 0.0f;
	}
}