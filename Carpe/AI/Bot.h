// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Player/CharacterBase.h"
#include "Bot.generated.h"

class UGameDamageType;

UENUM()
enum class EBotBehaviorType : uint8
{
	/* Does not move, remains in place until a player is spotted */
	Passive,

	/* Patrols a region until a player is spotted */
	Patrolling,
};

UCLASS()
class ABot : public ACharacterBase
{
	GENERATED_UCLASS_BODY()

private:
	float LastMeleeAttackTime;
	float MeleeStrikeCooldown;
	ACharacter* CurrentMeleeTarget;

	UPROPERTY(VisibleAnywhere, Category = "CFG|Attacking")
	UCapsuleComponent* MeleeCollisionComp;

	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

	/* A pawn is in melee range */
	UFUNCTION()
	void OnMeleeCompBeginOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult & SweepResult);

	void OnRetriggerMeleeStrike();

	UPROPERTY(EditAnywhere, Category = "CFG|Attacking")
	TSubclassOf<UDamageType> PunchDamageType;

	UPROPERTY(EditAnywhere, Category = "CFG|Attacking")
	float MeleeDamage;

	/* Timer handle to manage continous melee attacks while in range of a player */
	FTimerHandle TimerHandle_MeleeAttack;

	virtual void PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser) override;

public:

	UPROPERTY(EditAnywhere, Category = "CFG|AI")
	class UBehaviorTree* BotBehavior;

	//UPROPERTY(EditAnywhere, Category = "CFG|AI")
	EBotBehaviorType BotType;

	UPROPERTY(VisibleAnywhere, Category = "CFG|AI")
	class UPawnSensingComponent* PawnSensingComp;

	/* Deal damage to the Actor that was hit by the punch animation */
	UFUNCTION(BlueprintCallable, Category = "CFG|Attacking")
	void PerformMeleeStrike(AActor* HitActor);

	virtual bool IsFirstPerson() const override;

	virtual void FaceRotation(FRotator NewRotation, float DeltaTime = 0.f) override;

	UPawnNoiseEmitterComponent* NoiseEmitterComp;

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category = "CFG|Attacking")
	bool bIsPunching;
};