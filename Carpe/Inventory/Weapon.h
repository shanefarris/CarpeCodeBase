#pragma once

#include "InventoryItem.h"
#include "Runtime/Engine/Classes/Engine/Canvas.h"
#include "Weapon.generated.h"

USTRUCT(BlueprintType)
struct FWeaponStruct
{
	GENERATED_BODY()
public:

	/** inifite ammo for reloads */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Ammo")
		bool bInfiniteAmmo;

	/** infinite ammo in clip, no reload required */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Ammo")
		bool bInfiniteClip;

	/** max ammo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Ammo")
		int32 MaxAmmo;

	/** clip size */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Ammo")
		int32 AmmoPerClip;

	/** initial clips */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Ammo")
		int32 InitialClips;

	/** time between two consecutive shots */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|WeaponStat")
		float TimeBetweenShots;

	/** failsafe reload duration if weapon doesn't have any animation for it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|WeaponStat")
		float NoAnimReloadDuration;

	/** defaults */
	FWeaponStruct()
	{
		bInfiniteAmmo = false;
		bInfiniteClip = false;
		MaxAmmo = 100;
		AmmoPerClip = 20;
		InitialClips = 4;
		TimeBetweenShots = 0.2f;
		NoAnimReloadDuration = 1.0f;
	}
};

USTRUCT()
struct FWeaponAnimation
{
	GENERATED_USTRUCT_BODY()

	/** animation played on pawn (1st person view) */
	UPROPERTY(EditDefaultsOnly, Category = "CFG|Animation")
	UAnimMontage* Pawn1P;

	/** animation played on pawn (3rd person view) */
	UPROPERTY(EditDefaultsOnly, Category = "CFG|Animation")
	UAnimMontage* Pawn3P;
};

UCLASS(Abstract, Blueprintable)
class AWeapon : public AInventoryItem
{
	GENERATED_UCLASS_BODY()

	/** perform initial setup */
	virtual void PostInitializeComponents() override;

	virtual void Destroyed() override;

	//////////////////////////////////////////////////////////////////////////
	// Ammo
	enum class EAmmoType
	{
		EBullet,
		ERocket,
		EMax,
	};

	UFUNCTION(BlueprintCallable, Category = "CFG|Inventory")
	void GiveAmmo(int32 AddAmount);											/** [server] add ammo */

	void UseAmmo();															/** consume a bullet */
	virtual EAmmoType GetAmmoType() const { return EAmmoType::EBullet; }	/** query ammo type */

																			//////////////////////////////////////////////////////////////////////////
																			// Inventory
	virtual void OnEquip(const AWeapon* LastWeapon);						/** weapon is being equipped by owner pawn */
	virtual void OnEquipFinished();											/** weapon is now equipped by owner pawn */
	void OnUnEquip() override;												/** weapon is holstered by owner pawn */
	bool IsEquipped() const;												/** check if it's currently equipped */
	void UpdateMeshes();													/** update the meshes visibility */
	virtual bool CanBePickedUp(ACharacterBase* TestPawn) const override;	/** check if pawn can use this pickup */
	virtual void GivePickupTo(ACharacterBase* Pawn) override;				/** give pickup, this is the on pickup method */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	TSubclassOf<AWeapon> WeaponType;										/** which weapon gets ammo? */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	bool bIsEquipOnPickup;													/** which weapon gets ammo? */

																				//////////////////////////////////////////////////////////////////////////
																				// Input
	virtual void StartFire();												/** [local + server] start weapon fire */
	virtual void StopFire();												/** [local + server] stop weapon fire */
	virtual void StartReload(bool bFromReplication = false);				/** [all] start weapon reload */
	virtual void StopReload();												/** [local + server] interrupt weapon reload */
	virtual void ReloadWeapon();											/** [server] performs actual reload */

																			/** trigger reload from server */
	UFUNCTION(reliable, client)
	void ClientStartReload();

	//////////////////////////////////////////////////////////////////////////
	// Control
	bool CanFire() const;													/** check if weapon can fire */
	bool CanReload() const;													/** check if weapon can be reloaded */

																			//////////////////////////////////////////////////////////////////////////
																			// Reading data
	USkeletalMeshComponent* GetWeaponMesh() const;							/** get weapon mesh (needs pawn owner to determine variant) */
	bool HasInfiniteAmmo() const;											/** check if weapon has infinite ammo (include owner's cheats) */
	bool HasInfiniteClip() const;											/** check if weapon has infinite clip (include owner's cheats) */
	float GetEquipStartedTime() const;										/** gets last time when this weapon was switched to */
	float GetEquipDuration() const;											/** gets the duration of equipping weapon*/

	UFUNCTION(BlueprintCallable, Category = "CFG|Weapon")
	EInventoryState::Type GetCurrentState() const;								/** get current weapon state */

	UFUNCTION(BlueprintCallable, Category = "CFG|Weapon")
	int32 GetReserveAmmo() const;											/** get current ammo amount (total) */

	UFUNCTION(BlueprintCallable, Category = "CFG|Weapon")
	int32 GetCurrentAmmo() const;											/** get current ammo amount (total) */

	UFUNCTION(BlueprintCallable, Category = "CFG|Weapon")
	int32 GetCurrentAmmoInClip() const;										/** get current ammo amount (clip) */

	UFUNCTION(BlueprintCallable, Category = "CFG|Weapon")
	int32 GetAmmoPerClip() const;											/** get clip size */

	UFUNCTION(BlueprintCallable, Category = "CFG|Weapon")
	int32 GetMaxAmmo() const;												/** get max ammo amount */

	UFUNCTION(BlueprintCallable, Category = "CFG|Weapon")
	class ACharacterBase* GetPawnOwner() const;								/** get pawn owner */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|HUD")
	FSlateBrush PrimaryIcon;												/** icon displayed on the HUD when weapon is equipped as primary */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|HUD")
	FSlateBrush SecondaryIcon;												/** icon displayed on the HUD when weapon is secondary */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|HUD")
	FSlateBrush PrimaryClipIcon;											/** bullet icon used to draw current clip (left side) */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|HUD")
	FSlateBrush SecondaryClipIcon;											/** bullet icon used to draw secondary clip (left side) */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|HUD")
	float AmmoIconsCount;													/** how many icons to draw per clip */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|HUD")
	int32 PrimaryClipIconOffset;											/** defines spacing between primary ammo icons (left side) */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|HUD")
	int32 SecondaryClipIconOffset;											/** defines spacing between secondary ammo icons (left side) */

	UPROPERTY(EditAnywhere, Category = "CFG|HUD")
	FCanvasIcon Crosshair[5];												/** crosshair parts icons (left, top, right, bottom and center) */

	UPROPERTY(EditAnywhere, Category = "CFG|HUD")
	FCanvasIcon AimingCrosshair[5];											/** crosshair parts icons when targeting (left, top, right, bottom and center) */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|HUD")
	bool UseLaserDot;														/** only use red colored center part of aiming crosshair */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|HUD")
	bool UseCustomCrosshair;												/** false = default crosshair */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|HUD")
	bool UseCustomAimingCrosshair;											/** false = use custom one if set, otherwise default crosshair */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|HUD")
	bool bHideCrosshairWhileNotAiming;										/** true - crosshair will not be shown unless aiming with the weapon */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Config")
	FWeaponStruct WeaponConfig;												/** weapon data */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Config")
	float HeadShotMultiplyer;												/** how much more damage should head shots cause */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Config")
	bool bIsAutoReload;														/** automatically reload the weapon if we run out of ammo */

protected:
	uint32 bPlayingFireAnim : 1;											/** is fire animation playing? */
	uint32 bWantsToFire : 1;												/** is weapon fire active? */
	uint32 bRefiring;														/** weapon is refiring */
	EInventoryState::Type CurrentState;										/** current weapon state */
	float LastFireTime;														/** time of last successful weapon fire */
	float EquipStartedTime;													/** last time when this weapon was switched to */
	float EquipDuration;													/** how much time weapon needs to be equipped */

	FTimerHandle TimerHandle_OnEquipFinished;								/** Handle for efficient management of OnEquipFinished timer */
	FTimerHandle TimerHandle_StopReload;									/** Handle for efficient management of StopReload timer */
	FTimerHandle TimerHandle_ReloadWeapon;									/** Handle for efficient management of ReloadWeapon timer */
	FTimerHandle TimerHandle_HandleFiring;									/** Handle for efficient management of HandleFiring timer */

	UPROPERTY(Transient)
	UAudioComponent* FireAC;												/** firing audio (bLoopedFireSound set) */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Effects")
	FName MuzzleAttachPoint;												/** name of bone/socket for muzzle in weapon mesh */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Effects")
	UParticleSystem* MuzzleFX;												/** FX for muzzle flash */

	UPROPERTY(Transient)
	UParticleSystemComponent* MuzzlePSC;									/** spawned component for muzzle FX */

	UPROPERTY(Transient)
	UParticleSystemComponent* MuzzlePSCSecondary;							/** spawned component for second muzzle FX (Needed for split screen) */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Effects")
	TSubclassOf<UCameraShake> FireCameraShake;								/** camera shake on firing */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Effects")
	UForceFeedbackEffect *FireForceFeedback;								/** force feedback effect to play when the weapon is fired */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Sound")
	USoundCue* FireSound;													/** single fire sound (bLoopedFireSound not set) */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Sound")
	USoundCue* FireLoopSound;												/** looped fire sound (bLoopedFireSound set) */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Sound")
	USoundCue* FireFinishSound;												/** finished burst sound (bLoopedFireSound set) */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Sound")
	USoundCue* OutOfAmmoSound;												/** out of ammo sound */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Sound")
	USoundCue* ReloadSound;													/** reload sound */

	UPROPERTY(EditDefaultsOnly, Category = "CFG|AA_Required")
	FWeaponAnimation ReloadAnim;											/** reload animations */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Sound")
	USoundCue* EquipSound;													/** equip sound */

	UPROPERTY(EditDefaultsOnly, Category = "CFG|AA_Required")
	FWeaponAnimation EquipAnim;												/** equip animations */

	UPROPERTY(EditDefaultsOnly, Category = "CFG|AA_Required")
	FWeaponAnimation FireAnim;												/** fire animations */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Effects")
	uint32 bLoopedMuzzleFX : 1;												/** is muzzle FX looped? */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Sound")
	uint32 bLoopedFireSound : 1;											/** is fire sound looped? */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Animation")
	uint32 bLoopedFireAnim : 1;												/** is fire animation looped? */

	UPROPERTY(Transient, ReplicatedUsing = OnRep_Reload)
	uint32 bPendingReload : 1;												/** is reload animation playing? */

	UPROPERTY(Transient, Replicated)
	int32 ReserveAmmo;														/** current total ammo */

	UPROPERTY(Transient, Replicated)
	int32 CurrentAmmo;														/** current total ammo */

	UPROPERTY(Transient, Replicated)
	int32 CurrentAmmoInClip;												/** current ammo - inside clip */

	UPROPERTY(Transient, ReplicatedUsing = OnRep_BurstCounter)
	int32 BurstCounter;														/** burst counter, used for replicating fire events to remote clients */

																				//////////////////////////////////////////////////////////////////////////
																				// Input - server side
	UFUNCTION(reliable, server, WithValidation)
	void ServerStartFire();

	UFUNCTION(reliable, server, WithValidation)
	void ServerStopFire();

	UFUNCTION(reliable, server, WithValidation)
	void ServerStartReload();

	UFUNCTION(reliable, server, WithValidation)
	void ServerStopReload();

	//////////////////////////////////////////////////////////////////////////
	// Replication & effects
	UFUNCTION()
	void OnRep_BurstCounter();

	UFUNCTION()
	void OnRep_Reload();

	virtual void SimulateWeaponFire();										/** Called in network play to do the cosmetic fx for firing */
	virtual void StopSimulatingWeaponFire();								/** Called in network play to stop cosmetic fx (e.g. for a looping shot). */

																			//////////////////////////////////////////////////////////////////////////
																			// Weapon usage
	UFUNCTION(reliable, server, WithValidation)
	void ServerHandleFiring();												/** [server] fire & update ammo */

	void HandleFiring();													/** [local + server] handle weapon fire */
	virtual void OnBurstStarted();											/** [local + server] firing started */
	virtual void OnBurstFinished();											/** [local + server] firing finished */
	virtual void FireWeapon() PURE_VIRTUAL(AWeapon::FireWeapon, );			/** [local] weapon specific fire implementation */
	void SetWeaponState(EInventoryState::Type NewState);						/** update weapon state */
	void DetermineWeaponState();											/** determine current weapon state */
	void AttachMeshToPawn();												/** attaches weapon mesh to pawn's mesh */

																			//////////////////////////////////////////////////////////////////////////
																			// Weapon usage helpers
	UAudioComponent* PlayWeaponSound(USoundCue* Sound);						/** play weapon sounds */
	float PlayWeaponAnimation(const FWeaponAnimation& Animation);			/** play weapon animations */
	void StopWeaponAnimation(const FWeaponAnimation& Animation);			/** stop playing weapon animations */
	virtual FVector GetAdjustedAim() const;									/** Get the aim of the weapon, allowing for adjustments to be made by the weapon */
	FVector GetCameraAim() const;											/** Get the aim of the camera */
	FVector GetCameraDamageStartLocation(const FVector& AimDir) const;		/** get the originating location for camera damage */
	FVector GetMuzzleLocation() const;										/** get the muzzle location of the weapon */
	FVector GetMuzzleDirection() const;										/** get direction of weapon's muzzle */

	bool WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo, FHitResult& HitResult) const;/** find hit */
	bool WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo, TArray<FHitResult>& HitResults) const;

	FORCEINLINE USkeletalMeshComponent* GetMesh1P() const { return MeshMain; }/** Returns Mesh1P subobject **/
};

