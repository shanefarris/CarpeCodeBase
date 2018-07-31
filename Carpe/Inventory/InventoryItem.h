#pragma once

#include "InventoryItem.generated.h"

class UAnimSequence;
class UBlendSpace1D;
class UChildActorComponent;
class ACharacterBase;
class USoundCue;
class USphereComponent;

UENUM()
enum class EInventoryType : uint8
{
	Weapon,
	Magazine,
	Projectile,
	Melee,
	Tool,
	Health,
	Food,
};

UENUM(BlueprintType)
namespace EInventoryState
{
	enum Type
	{
		Idle,
		Firing,
		Reloading,
		Equipping,
		OutOfAmmo,
	};
}

// Base class for inventory items placed in the world or in the players inventory system
UCLASS(Abstract, Blueprintable)
class AInventoryItem : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	virtual void OnEnterInventory(ACharacterBase* NewOwner);// [server] Item was added to pawn's inventory				
	virtual void OnLeaveInventory();				// [server] Item was removed from pawn's inventory
	virtual void OnUnEquip();						// Item is holstered by owner pawn 
	virtual void OnBeginFocus();					// Player is looking at
	virtual void OnEndFocus();						// Player is no longer looking at
	virtual void OnUsed(APawn* InstigatorPawn);		// Called when player interacts with object
	virtual void NotifyActorBeginOverlap(class AActor* Other) override;	// Pickup on touch 
	virtual bool CanBePickedUp(class ACharacterBase* TestPawn) const;	// check if pawn can use this pickup
	virtual void BeginPlay() override;				// Initial setup
	virtual void GivePickupTo(class ACharacterBase* Pawn);	// Give pickup
	void SetOwningPawn(ACharacterBase* ACharacterBase);	// Set the Item's owning pawn

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|AA_Required")
		EInventoryType InventoryType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|AA_Required")	// Use item
		UAnimSequence* PlayerUseAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|AA_Required")	// Reload/recharge, etc item.
		UAnimSequence* PlayerReloadAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|AA_Required")	// These 2 are blends because of the speed variable
		UBlendSpace1D* PlayerLocomotionAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG|AA_Required")	// In the air/jumping/falling
		UAnimSequence* PlayerInAirAnim;

protected:
	bool bIsFocused;
	uint32 bIsEquipped : 1;
	uint32 bPendingEquip : 1;
	FTimerHandle TimerHandle_RespawnPickup;							// Handle for efficient management of RespawnPickup timer

	UPROPERTY(Transient, ReplicatedUsing = OnRep_MyPawn)
		class ACharacterBase* MyPawn;									/** pawn owner */

	UPROPERTY(EditAnywhere, Category = "CFG|AA_Required")
		USkeletalMeshComponent* MeshMain;								/** main view when item is in 'use' */

	UPROPERTY(VisibleAnywhere, Category = "CFG|AA_Required")
		UStaticMeshComponent* MeshWorld;								/* The mesh used while on the world map, and un-used. */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
		UParticleSystemComponent* PickupPSC;							/** FX component */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
		UParticleSystem* ActiveFX;										/** FX of active pickup */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
		UParticleSystem* RespawningFX;									/** FX of pickup on respawn timer */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
		USoundCue* PickupSound;											/** sound played when player picks it up */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
		USoundCue* RespawnSound;										/** sound played on respawn */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
		float RespawnTime;												/** how long it takes to respawn, 0.0 if it does not respawn */

	UPROPERTY(Transient, ReplicatedUsing = OnRep_IsActive)
		uint32 bIsActive : 1;												/** is it ready for interactions? */

	UPROPERTY(Transient, Replicated)
		ACharacterBase* PickedUpBy;										/* The character who has picked up this pickup */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
		bool bIsPickupOnTouch;											/** do we get this if we run over it, then true, otherise false if we need to manually pick it up */

	UFUNCTION()
		void OnRep_IsActive();

	void PickupOnTouch(class ACharacterBase* Pawn);					// Handle touches
	virtual void RespawnPickup();									// Show and enable pickup
	virtual void OnPickedUp();										// Sow effects when pickup disappears
	virtual void OnRespawned();										// Show effects when pickup appears
	bool IsAttachedToPawn() const;									// Check if mesh is already attached
	void DetachMeshFromPawn();
	FORCEINLINE UParticleSystemComponent* GetPickupPSC() const { return PickupPSC; }
	FORCEINLINE UStaticMeshComponent* GetMeshComponent() const { return MeshWorld; }

	UFUNCTION(BlueprintImplementableEvent)
		void OnPickedUpEvent();											/** blueprint event: pickup disappears */

	UFUNCTION(BlueprintImplementableEvent)
		void OnRespawnEvent();											/** blueprint event: pickup appears */

																		// Bounding sphere for pickup message
	bool bIsOverlapped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
		USphereComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
		FString InteractiveMessage;

	UFUNCTION(BlueprintCallable, Category = "CFG")
		void TriggerEnter(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable, Category = "CFG")
		void TriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//////////////////////////////////////////////////////////////////////////
	// Replication & effects
	UFUNCTION()
		void OnRep_MyPawn();//*****************
};