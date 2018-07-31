// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Weapon_Projectile.h"

#include "Projectile.generated.h"

class UProjectileMovementComponent;
 
UCLASS(Abstract, Blueprintable)
class AProjectile : public AActor
{
	GENERATED_UCLASS_BODY()

protected:
	/** initial setup */
	virtual void PostInitializeComponents() override;

	/** handle hit */
	UFUNCTION()
	void OnImpact(const FHitResult& HitResult);

	/** movement component */
	UPROPERTY(VisibleDefaultsOnly, Category= "CFG|Projectile")
	UProjectileMovementComponent* MovementComp;

	/** collisions */
	UPROPERTY(VisibleDefaultsOnly, Category= "CFG|Projectile")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category= "CFG|Projectile")
	UParticleSystemComponent* ParticleComp;

	/** effects for explosion */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Effects")
	TSubclassOf<class AExplosionEffect> ExplosionTemplate;

	/** controller that fired me (cache for damage calculations) */
	TWeakObjectPtr<AController> MyController;

	/** projectile data */
	struct FProjectileData WeaponConfig;

	/** did it explode? */
	UPROPERTY(Transient, ReplicatedUsing=OnRep_Exploded)
	bool bExploded;

	/** [client] explosion happened */
	UFUNCTION()
	void OnRep_Exploded();

	/** trigger explosion */
	void Explode(const FHitResult& Impact);

	/** shutdown projectile and prepare for destruction */
	void DisableAndDestroy();

	/** update velocity on client */
	virtual void PostNetReceiveVelocity(const FVector& NewVelocity) override;

	/** Returns MovementComp subobject **/
	FORCEINLINE UProjectileMovementComponent* GetMovementComp() const { return MovementComp; }
	/** Returns CollisionComp subobject **/
	FORCEINLINE USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ParticleComp subobject **/
	FORCEINLINE UParticleSystemComponent* GetParticleComp() const { return ParticleComp; }

public:
	/** setup velocity */
	void InitVelocity(FVector& ShootDirection);
};
