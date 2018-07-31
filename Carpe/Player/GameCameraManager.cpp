#include "Player/GameCameraManager.h"
#include "Carpe.h"

AGameCameraManager::AGameCameraManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NormalFOV = 90.0f;
	TargetingFOV = 60.0f;
	ViewPitchMin = -87.0f;
	ViewPitchMax = 87.0f;
	bAlwaysApplyModifiers = true;
}

void AGameCameraManager::UpdateCamera(float DeltaTime)
{
	ACharacterBase* MyPawn = PCOwner ? Cast<ACharacterBase>(PCOwner->GetPawn()) : NULL;
	if (MyPawn && MyPawn->IsAlive() && MyPawn->IsLocallyControlled())
	{
		const float TargetFOV = MyPawn->IsTargeting() ? TargetingFOV : NormalFOV;
		DefaultFOV = FMath::FInterpTo(DefaultFOV, TargetFOV, DeltaTime, 20.0f);
	}

	Super::UpdateCamera(DeltaTime);

	if (MyPawn && MyPawn->IsAlive() && MyPawn->IsLocallyControlled())
	{
		MyPawn->OnCameraUpdate(GetCameraLocation(), GetCameraRotation());
	}
}
