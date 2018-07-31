#pragma once

#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "AnimInstanceBase.generated.h"

UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class UAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()
public:
	APawn* OwningPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	bool IsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	float JumpTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	bool IsRunning;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	bool IsWalking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	bool IsFiring;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	bool IsTargeting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	bool IsReloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	float AimYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	float AimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	bool IsRifle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	UAnimSequence* CurrentInventoryItemUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	UBlendSpace1D* CurrentInventoryItemLocomotion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	UAnimSequence* CurrentInventoryItemReload;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

};