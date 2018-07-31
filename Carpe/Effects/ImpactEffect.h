// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Types.h"
#include "ImpactEffect.generated.h"

//
// Spawnable effect for weapon hit impact - NOT replicated to clients
// Each impact type should be defined as separate blueprint
//
UCLASS(Abstract, Blueprintable)
class AImpactEffect : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	// Particals (visual) hit impacts
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Visual")
	UParticleSystem* DefaultFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Visual")
	UParticleSystem* ConcreteFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Visual")
	UParticleSystem* DirtFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Visual")
	UParticleSystem* WaterFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Visual")
	UParticleSystem* MetalFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Visual")
	UParticleSystem* WoodFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Visual")
	UParticleSystem* GlassFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Visual")
	UParticleSystem* GrassFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Visual")
	UParticleSystem* BodyFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Visual")
	UParticleSystem* HeadFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Visual")
	UParticleSystem* LimbFX;

	// Sound impacts
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Sound")
	USoundCue* DefaultSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Sound")
	USoundCue* ConcreteSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Sound")
	USoundCue* DirtSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Sound")
	USoundCue* WaterSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Sound")
	USoundCue* MetalSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Sound")
	USoundCue* WoodSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Sound")
	USoundCue* GlassSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Sound")
	USoundCue* GrassSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Sound")
	USoundCue* BodySound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Sound")
	USoundCue* HeadSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Sound")
	USoundCue* LimbSound;

	// Default decal
	UPROPERTY(EditDefaultsOnly, Category= "CFG|Decal")
	struct FDecalStruct DefaultDecal;

	// Surface data for spawning
	UPROPERTY(BlueprintReadOnly, Category= "CFG|Surface")
	FHitResult SurfaceHit;

	/** spawn effect */
	virtual void PostInitializeComponents() override;

protected:

	UParticleSystem* GetImpactFX(TEnumAsByte<EPhysicalSurface> SurfaceType) const;
	USoundCue* GetImpactSound(TEnumAsByte<EPhysicalSurface> SurfaceType) const;
};
