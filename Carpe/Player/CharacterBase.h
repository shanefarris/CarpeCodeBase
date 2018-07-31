// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

//#include "CoreDefines.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "CharacterBase.generated.h"

class AInventoryItem;
class UBlendSpace1D;
class USkeletalMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class USoundCue;

UENUM()
enum class ECharacterType : uint8
{
	Player,
	Shooter1,
	Zombie1,
};

/** replicated information on a hit we've taken */
USTRUCT()
struct FHitInfo
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY()
		float ActualDamage;										/** The amount of damage actually applied */

	UPROPERTY()
		UClass* DamageTypeClass;								/** The damage type we were hit with. */

	UPROPERTY()
		TWeakObjectPtr<class ACharacterBase> PawnInstigator;	/** Who hit us */

	UPROPERTY()
		TWeakObjectPtr<class AActor> DamageCauser;				/** Who actually caused the damage */

	UPROPERTY()
		int32 DamageEventClassID;								/** Specifies which DamageEvent below describes the damage received. */

	UPROPERTY()
		uint32 bKilled : 1;										/** Rather this was a kill */

private:
	UPROPERTY()
		uint8 EnsureReplicationByte;							/** A rolling counter used to ensure the struct is dirty and will replicate. */

	UPROPERTY()
		FDamageEvent GeneralDamageEvent;						/** Describes general damage. */

	UPROPERTY()
		FPointDamageEvent PointDamageEvent;						/** Describes point damage, if that is what was received. */

	UPROPERTY()
		FRadialDamageEvent RadialDamageEvent;					/** Describes radial damage, if that is what was received. */

public:
	FHitInfo() : ActualDamage(0), DamageTypeClass(nullptr), PawnInstigator(nullptr), DamageCauser(nullptr), DamageEventClassID(0), bKilled(false), EnsureReplicationByte(0) {}

	FDamageEvent& GetDamageEvent()
	{
		switch (DamageEventClassID)
		{
		case FPointDamageEvent::ClassID:
			if (PointDamageEvent.DamageTypeClass == nullptr)
			{
				PointDamageEvent.DamageTypeClass = DamageTypeClass ? DamageTypeClass : UDamageType::StaticClass();
			}
			return PointDamageEvent;

		case FRadialDamageEvent::ClassID:
			if (RadialDamageEvent.DamageTypeClass == nullptr)
			{
				RadialDamageEvent.DamageTypeClass = DamageTypeClass ? DamageTypeClass : UDamageType::StaticClass();
			}
			return RadialDamageEvent;

		default:
			if (GeneralDamageEvent.DamageTypeClass == nullptr)
			{
				GeneralDamageEvent.DamageTypeClass = DamageTypeClass ? DamageTypeClass : UDamageType::StaticClass();
			}
			return GeneralDamageEvent;
		}
	}

	void SetDamageEvent(const FDamageEvent& DamageEvent)
	{
		DamageEventClassID = DamageEvent.GetTypeID();
		switch (DamageEventClassID)
		{
		case FPointDamageEvent::ClassID:
			PointDamageEvent = *((FPointDamageEvent const*)(&DamageEvent));
			break;
		case FRadialDamageEvent::ClassID:
			RadialDamageEvent = *((FRadialDamageEvent const*)(&DamageEvent));
			break;
		default:
			GeneralDamageEvent = DamageEvent;
		}

		DamageTypeClass = DamageEvent.DamageTypeClass;
	}

	void EnsureReplication()
	{
		EnsureReplicationByte++;
	}
};

USTRUCT()
struct FCharacterAttributesStruct
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "CFG")
	float SprintSpeed;														/** How fast can the character sprint */

	UPROPERTY(EditDefaultsOnly, Category = "CFG")
	float MaxHealth;														/** Max health */

	FCharacterAttributesStruct()
	{
		SprintSpeed = 5.0f;
		MaxHealth = 100.0f;
	}
};

UCLASS(Abstract)
class ACharacterBase : public ACharacter
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "CFG")
	FCharacterAttributesStruct CharacterAttributes;							/** Character attributes */

	//////////////////////////////////////////////////////////////////////////
	// Damage & death	
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;/** Take damage, handle death */
	virtual void Suicide();													/** Pawn suicide */
	virtual void KilledBy(class APawn* EventInstigator);					/** Kill this pawn */
	virtual bool CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const;/** Returns True if the pawn can die in the current state */
	virtual bool Die(float KillingDamage, struct FDamageEvent const& DamageEvent, class AController* Killer, class AActor* DamageCauser);/** Kills pawn.  Server/authority only. */
	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;	// Die when we fall out of the world.	
	virtual void PreReplication(IRepChangedPropertyTracker & ChangedPropertyTracker) override;	/** Called on the actor right before replication occurs */
	virtual void PostInitializeComponents() override;						/** spawn inventory, setup initial variables */
	virtual void Tick(float DeltaSeconds) override;							/** Update the character. (Running, health etc). */
	virtual void Destroyed() override;										/** cleanup inventory */
	virtual void PawnClientRestart() override;								/** update mesh for first person view */
	virtual void PossessedBy(class AController* C) override;				/** [server] perform PlayerState related setup */
	virtual void OnRep_PlayerState() override;								/** [client] perform PlayerState related setup */
	void OnCameraUpdate(const FVector& CameraLocation, const FRotator& CameraRotation);	/** Add camera pitch to first person mesh. */
	bool IsEnemyFor(AController* TestPC) const;								/** Check if pawn is enemy if given controller. */

	UPROPERTY(EditAnywhere, Category = "CFG")
	ECharacterType CharacterType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "CFG")
	bool RagdollOnDeath;													/** Will be go to ragdoll on death, and after the death montage (if any) */

	UFUNCTION(BlueprintCallable, Category="CFG|Game|Weapon")
	FRotator GetAimOffsets() const;											/** get aim offsets */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CFG")
	bool bIsDying;															/** Identifies if pawn is in its dying state */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "CFG")
	float Health;															// Current health of the Pawn

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	float ReloadSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "CFG")
	bool bIsReloading;														// Current health of the Pawn

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "CFG")
	bool bPlayDeathAnim = false;											// Current health of the Pawn

	//////////////////////////////////////////////////////////////////////////
	// Inventory
	void AddWeapon(class AWeapon* Weapon);									/** [server] add weapon to inventory */
	void RemoveWeapon(class AWeapon* Weapon);								/** [server] remove weapon from inventory */
	class AWeapon* FindWeapon(TSubclassOf<class AWeapon> WeaponClass);		/** Find in inventory */
	void EquipWeapon(class AWeapon* Weapon);								/** [server + local] equips weapon from inventory */

	//////////////////////////////////////////////////////////////////////////
	// Weapon usage
	void StartWeaponFire();													/** [local] starts weapon fire */
	void StopWeaponFire();													/** [local] stops weapon fire */
	bool CanFire() const;													/** check if pawn can fire weapon */
	bool CanReload() const;													/** check if pawn can reload weapon */
	void SetTargeting(bool bNewTargeting);									/** [server + local] change targeting state */

	//////////////////////////////////////////////////////////////////////////
	// Movement
	void SetRunning(bool bNewRunning, bool bToggle);						/** [server + local] change running state */
	
	//////////////////////////////////////////////////////////////////////////
	// Animations
	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None) override; /** play anim montage */
	virtual void StopAnimMontage(class UAnimMontage* AnimMontage) override;	/** stop playing montage */
	void StopAllAnimMontages();												/** stop playing all montages */

	//////////////////////////////////////////////////////////////////////////
	// Input handlers
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;	/** setup pawn specific input handlers */
	void MoveForward(float Val);											/** Move forward/back */
	void MoveRight(float Val);												/** Strafe right/left */	
	void MoveUp(float Val);													/** Move Up/Down in allowed movement modes. */	
	void TurnAtRate(float Val);												/* Frame rate independent turn */
	void LookUpAtRate(float Val);											/* Frame rate independent lookup */
	void OnStartFire();														/** player pressed start fire action */
	void OnStopFire();														/** player released start fire action */
	void OnStartTargeting();												/** player pressed targeting action */
	void OnStopTargeting();													/** player released targeting action */
	void OnNextWeapon();													/** player pressed next weapon action */
	void OnPrevWeapon();													/** player pressed prev weapon action */
	void OnReload();														/** player pressed reload action */
	void OnStartJump();														/** player pressed jump action */
	void OnStopJump();														/** player released jump action */
	void OnStartRunning();													/** player pressed run action */
	void OnStartRunningToggle();											/** player pressed toggled run action */
	void OnStopRunning();													/** player released run action */
	void OnAction();														/** player released run action */
	void OnThirdPerson();													/** player pressed 3rd person action */
	void OnThirdPersonToggle();												/** player toggled 3rd person action */
	void OnFirstPerson();													/** player released 3rd person action */

	//////////////////////////////////////////////////////////////////////////
	// Reading data
	USkeletalMeshComponent* GetPawnMesh() const;							/** get mesh component */
	int32 GetMaxHealth() const;												/** get max health */
	bool IsAlive() const;													/** check if pawn is still alive */
	AActor* GetActorInView();
	float GetLowHealthPercentage() const;									/** returns percentage of health when low health effects should start */
	USkeletalMeshComponent* GetSpecifcPawnMesh(bool WantFirstPerson) const;/** Get either first or third person mesh. */
	void UpdateTeamColorsAllMIDs();											/** Update the team color of all player meshes. */
	FName GetWeaponAttachPoint() const;										/** get weapon attach point */
	int32 GetInventoryCount() const;										/** get total number of inventory items */
	class AWeapon* GetInventoryWeapon(int32 index) const;					/** get weapon from inventory at index. Index validity is not checked. */

	UFUNCTION(BlueprintCallable, Category="CFG|Game|Weapon")
	class AWeapon* GetWeapon() const;										/** get currently equipped weapon */

	UFUNCTION(BlueprintCallable, Category="CFG|Game|Weapon")
	float GetTargetingSpeedModifier() const;								/** get weapon taget modifier speed	*/

	UFUNCTION(BlueprintCallable, Category="CFG|Game|Weapon")
	bool IsTargeting() const;												/** get targeting state */

	UFUNCTION(BlueprintCallable, Category="CFG|Game|Weapon")
	bool IsFiring() const;													/** get firing state */

	UFUNCTION(BlueprintCallable, Category="CFG|Pawn")
	float GetRunningSpeedModifier() const;									/** get the modifier value for running speed */

	UFUNCTION(BlueprintCallable, Category= "CFG|Pawn")
	bool IsRunning() const;													/** get running state */

	UFUNCTION(BlueprintCallable, Category= "CFG|Mesh")
	virtual bool IsFirstPerson() const;										/** get camera view type */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|ObjectInteraction")
	float MaxUseDistance;													/** Max distance to use/focus on actors. */

	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentWeapon)
	class AWeapon* CurrentWeapon;											/** currently equipped weapon */

protected:
	uint8 bWantsToRunToggled : 1;											/** from gamepad running is toggled */
	uint8 bWantsToFire : 1;													/** current firing state */
	float LowHealthPercentage;												/** when low health effects should start */
	float BaseTurnRate;														/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	float BaseLookUpRate;													/** Base lookup rate, in deg/sec. Other scaling may affect final lookup rate. */
	float LastSeenTime;														/* Last time the player was spotted */
	float LastHeardTime;													/* Last time the player was heard */
	float LastMeleeAttackTime;												/* Last time we attacked something */
	bool bSensedTarget;														/* Resets after sense time-out to avoid unnecessary clearing of target each tick */

	void UpdateRunSounds(bool bNewRunning);									/** handles sounds for running */
	void UpdatePawnMeshes();												/** handle mesh visibility and updates */
	void UpdateTeamColors(UMaterialInstanceDynamic* UseMID);				/** handle mesh colors on specified material instance */
	virtual void TornOff();													/** Responsible for cleaning up bodies on clients. */
	void UpdateCameraFirstPerson(const FVector& CameraLocation, const FRotator& CameraRotation);/** camera update in first person */
	void UpdateCameraThirdPerson(const FVector& CameraLocation, const FRotator& CameraRotation);/** camera update in third person */
	void TurnOff() override;
	virtual void OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser);/** notification when killed, for both the server and client. */
	virtual void PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser);/** play effects on hit */
	void SetRagdollPhysics();												/** switch to ragdoll */
	void ReplicateHit(float Damage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser, bool bKilled);/** sets up the replication for taking a hit */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Inventory")
	AInventoryItem* FocusedInventoryItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Inventory")
	FName WeaponAttachPoint;												/** socket or bone name for attaching weapon mesh */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Inventory")
	TArray<TSubclassOf<class AWeapon> > DefaultInventoryClasses;			/** default inventory list */

	UPROPERTY(Transient, Replicated)
	TArray<class AWeapon*> Inventory;										/** weapons in inventory */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Inventory")
	int32 MaxWeaponsCanCarry;												// The max number of weapons the character can carry.
	
	UPROPERTY(Transient, ReplicatedUsing=OnRep_LastTakeHitInfo)
	struct FHitInfo LastTakeHitInfo;										/** Replicate where this pawn was last hit and damaged */
	
	float LastTakeHitTimeTimeout;											/** Time at which point the last take hit info for the actor times out and won't be replicated; Used to stop join-in-progress effects all over the screen */
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Inventory")
	float TargetingSpeedModifier;											/** modifier for max movement speed */

	UPROPERTY(Transient, Replicated)
	uint8 bIsTargeting : 1;													/** current targeting state */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Pawn")
	float RunningSpeedModifier;												/** modifier for max movement speed */

	UPROPERTY(Transient, Replicated)
	uint8 bWantsToRun : 1;													/** current running state */

	UPROPERTY(Transient, Replicated)
	uint8 bIsThirdPerson;													/** flag used to toggle third person camera view */
	
	UPROPERTY(Transient)
	TArray<UMaterialInstanceDynamic*> MeshMIDs;								/** material instances for setting team color in mesh (3rd person view) */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Animation")
	UBlendSpace1D* IdleWalkBlendSpace;										/** Idle/walk locomotion for anim BP */
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CFG|Animation")
	UAnimMontage* DeathAnim;												/** animation played on death */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Pawn")
	USoundCue* DeathSound;													/** sound played on death, local player only */
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Pawn")
	UParticleSystem* RespawnFX;												/** effect played on respawn */
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Pawn")
	USoundCue* RespawnSound;												/** sound played on respawn */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Pawn")
	USoundCue* LowHealthSound;												/** sound played when health is low */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Pawn")
	USoundCue* RunLoopSound;												/** sound played when running */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Pawn")
	USoundCue* RunStopSound;												/** sound played when stop running */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "CFG|Pawn")
	USoundCue* TargetingSound;												/** sound played when targeting state changes */

	UPROPERTY()
	UAudioComponent* RunLoopAC;												/** used to manipulate with run loop sound */
	
	UPROPERTY()
	UAudioComponent* LowHealthWarningPlayer;								/** hook to looped low health sound used to stop/adjust volume */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|Mesh")
	USkeletalMeshComponent* Mesh1P;											/** pawn mesh: 1st person view */

	UPROPERTY(VisibleDefaultsOnly, Category = "CFG|Camera")
	UCameraComponent* ThirdPersonCamera;									/** third person camera */

	UPROPERTY(VisibleDefaultsOnly, Category = "CFG|Camera")
	USpringArmComponent* ThirdPersonCameraArm;								/** an arm for the third person camera*/

	UFUNCTION()
	void OnRep_LastTakeHitInfo();											/** play hit or death on client */

	//////////////////////////////////////////////////////////////////////////
	// Inventory
	UFUNCTION()
	void OnRep_CurrentWeapon(class AWeapon* LastWeapon);					/** current weapon rep handler */

	void SpawnDefaultInventory();											/** [server] spawns default inventory */
	void DestroyInventory();												/** [server] remove all weapons from inventory and destroy them */
	void DropWeapon(AWeapon* Weapon);										/** [server] drop (or destroy) weapon from inventory */
	void SetCurrentWeapon(class AWeapon* NewWeapon, class AWeapon* LastWeapon = nullptr);

	UFUNCTION(reliable, server, WithValidation)
	void ServerEquipWeapon(class AWeapon* NewWeapon);						/** equip weapon */

	UFUNCTION(reliable, server, WithValidation)
	void ServerSetTargeting(bool bNewTargeting);							/** update targeting state */

	UFUNCTION(reliable, server, WithValidation)
	void ServerSetRunning(bool bNewRunning, bool bToggle);					/** update targeting state */

	UFUNCTION(reliable, server, WithValidation)
	void ServerSetThirdPerson(bool bNewThirdPerson);						/** update first person state*/

	void SetThirdPerson(bool bNewThirdPerson);								/** <GB> [server + local] change third person state (server has to know that player is using third person) */	
	FORCEINLINE USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }/** Returns Mesh1P subobject **/
};

