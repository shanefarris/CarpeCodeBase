#include "ImpactEffect.h"
#include "Carpe.h"

AImpactEffect::AImpactEffect(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bAutoDestroyWhenFinished = true;
}

void AImpactEffect::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UPhysicalMaterial* HitPhysMat = SurfaceHit.PhysMaterial.Get();
	EPhysicalSurface HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitPhysMat);

	// show particles
	UParticleSystem* ImpactFX = GetImpactFX(HitSurfaceType);
	if (ImpactFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, GetActorLocation(), GetActorRotation());
	}

	// play sound
	USoundCue* ImpactSound = GetImpactSound(HitSurfaceType);
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}

	if (DefaultDecal.DecalMaterial)
	{
		FRotator RandomDecalRotation = SurfaceHit.ImpactNormal.Rotation();
		RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);

		UGameplayStatics::SpawnDecalAttached(DefaultDecal.DecalMaterial, FVector(1.0f, DefaultDecal.DecalSize, DefaultDecal.DecalSize),
			SurfaceHit.Component.Get(), SurfaceHit.BoneName,
			SurfaceHit.ImpactPoint, RandomDecalRotation, EAttachLocation::KeepWorldPosition,
			DefaultDecal.LifeSpan);
	}
}

UParticleSystem* AImpactEffect::GetImpactFX(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	switch (SurfaceType)
	{
		case GAME_SURFACE_Concrete:	
			return ConcreteFX;
		case GAME_SURFACE_Dirt:		
			return DirtFX;
		case GAME_SURFACE_Water:		
			return WaterFX;
		case GAME_SURFACE_Metal:		
			return MetalFX;
		case GAME_SURFACE_Wood:		
			return WoodFX;
		case GAME_SURFACE_Grass:		
			return GrassFX;
		case GAME_SURFACE_Glass:		
			return GlassFX;
		case GAME_SURFACE_Body:		
			return BodyFX;
		case GAME_SURFACE_Head:
			return HeadFX;
		case GAME_SURFACE_Limb:
			return LimbFX;
		default:						
			return DefaultFX;
	}
}

USoundCue* AImpactEffect::GetImpactSound(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	switch (SurfaceType)
	{
		case GAME_SURFACE_Concrete:	
			return ConcreteSound; 
		case GAME_SURFACE_Dirt:		
			return DirtSound; 
		case GAME_SURFACE_Water:		
			return WaterSound; 
		case GAME_SURFACE_Metal:		
			return MetalSound; 
		case GAME_SURFACE_Wood:		
			return WoodSound;
		case GAME_SURFACE_Grass:		
			return GrassSound;
		case GAME_SURFACE_Glass:		
			return GlassSound;
		case GAME_SURFACE_Body:		
			return BodySound;
		case GAME_SURFACE_Head:
			return HeadSound;
		case GAME_SURFACE_Limb:
			return LimbSound;
		default:						
			return DefaultSound; 
	}
}
