// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Types.h"
#include "ExplosionEffect.generated.h"

class UPointLightComponent;
class USoundCue;

//
// Spawnable effect for explosion - NOT replicated to clients
// Each explosion type should be defined as separate blueprint
//
UCLASS(Abstract, Blueprintable)
class AExplosionEffect : public AActor
{
	GENERATED_UCLASS_BODY()

private:
	/** explosion FX */
	UPROPERTY(EditAnywhere, Category= "CFG|Effect")
	UParticleSystem* ExplosionFX;

	/** explosion light */
	UPROPERTY(EditAnywhere, Category= "CFG|Effect")
	UPointLightComponent* ExplosionLight;

	/** Point light component name */
	FName ExplosionLightComponentName;

public:

	/** how long keep explosion light on? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Effect")
	float ExplosionLightFadeOut;

	/** explosion sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Effect")
	USoundCue* ExplosionSound;
	
	/** explosion decals */
	UPROPERTY(EditDefaultsOnly, Category= "CFG|Effect")
	struct FDecalStruct Decal;

	/** surface data for spawning */
	UPROPERTY(BlueprintReadOnly, Category= "CFG|Effect")
	FHitResult SurfaceHit;

	/** spawn explosion */
	virtual void BeginPlay() override;

	/** update fading light */
	virtual void Tick(float DeltaSeconds) override;

public:
	/** Returns ExplosionLight subobject **/
	FORCEINLINE UPointLightComponent* GetExplosionLight() const { return ExplosionLight; }
};
