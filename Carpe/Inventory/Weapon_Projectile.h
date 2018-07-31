// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Weapon.h"
#include "Weapon_Projectile.generated.h"

USTRUCT()
struct FProjectileData
{
	GENERATED_USTRUCT_BODY()

	/** projectile class */
	UPROPERTY(EditDefaultsOnly, Category= "CFG|WeaponStat")
	TSubclassOf<class AProjectile> ProjectileClass;

	/** life time */
	UPROPERTY(EditDefaultsOnly, Category= "CFG|WeaponStat")
	float ProjectileLife;

	/** damage at impact point */
	UPROPERTY(EditDefaultsOnly, Category="CFG|WeaponStat")
	int32 ExplosionDamage;

	/** radius of damage */
	UPROPERTY(EditDefaultsOnly, Category= "CFG|WeaponStat")
	float ExplosionRadius;

	/** type of damage */
	UPROPERTY(EditDefaultsOnly, Category= "CFG|WeaponStat")
	TSubclassOf<UDamageType> DamageType;

	/** defaults */
	FProjectileData()
	{
		ProjectileClass = NULL;
		ProjectileLife = 10.0f;
		ExplosionDamage = 100;
		ExplosionRadius = 300.0f;
		DamageType = UDamageType::StaticClass();
	}
};

// A weapon that fires a visible projectile
UCLASS(Abstract)
class AWeapon_Projectile : public AWeapon
{
	GENERATED_UCLASS_BODY()

	/** apply config on projectile */
	void ApplyWeaponConfig(FProjectileData& Data);

protected:

	virtual EAmmoType GetAmmoType() const override
	{
		return EAmmoType::ERocket;
	}

	/** weapon config */
	UPROPERTY(EditDefaultsOnly, Category="CFG|Config")
	FProjectileData ProjectileConfig;

	//////////////////////////////////////////////////////////////////////////
	// Weapon usage

	/** [local] weapon specific fire implementation */
	virtual void FireWeapon() override;

	/** spawn projectile on server */
	UFUNCTION(reliable, server, WithValidation)
	void ServerFireProjectile(FVector Origin, FVector_NetQuantizeNormal ShootDir);
};
