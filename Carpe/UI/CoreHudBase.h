#pragma once

#include "Types.h"
#include "CoreHudBase.generated.h"

class UCoreHudComponent;

struct FHitData
{
	/** Last hit time. */
	float HitTime;

	/** strength of hit icon */
	float HitPercentage;

	/** Initialise defaults. */
	FHitData()
	{
		HitTime = 0.0f;
		HitPercentage = 0.0f;
	}
};

/* Expose it to Blueprint using this tag */
UENUM(BlueprintType)
enum class EHUDState : uint8
{
	Playing,
	Spectating,
	MatchEnd
};

UENUM(BlueprintType)
enum class EHudNotifyType : uint8
{
	HealthChanged,
	AmmoChanged,
	WeaponChanged,
};

UCLASS(BlueprintType)
class UHudNotifyEvent : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	EHudNotifyType HudNotifyType;

	UFUNCTION(BlueprintImplementableEvent, Category = "CFG")
	void Execute(const FString& Param);
};

UCLASS()
class ACoreHudBase : public AHUD
{
	GENERATED_BODY()

public:

	/** Main HUD update loop. */
	virtual void DrawHUD() override;

	/**
	* Sent from pawn hit, used to calculate hit notification overlay for drawing.
	*
	* @param	DamageTaken		The amount of damage.
	* @param	DamageEvent		The actual damage event.
	* @param	PawnInstigator	The pawn that did the damage.
	*/
	void NotifyWeaponHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator);

	/** Notifies we have hit the enemy. */
	void NotifyEnemyHit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CFG")
	float CrosshairScale = 0.07f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	FString CurrentInteractiveMessage;

	/** On crosshair indicator that we hit someone. */
	UPROPERTY()
	FCanvasIcon HitNotifyCrosshair;

	/** Crosshair icons (left, top, right, bottom and center). */
	UPROPERTY()
	FCanvasIcon DefaultCrosshair[5];

	/** texture for HUD elements. */
	UPROPERTY()
	UTexture2D* HUDMainTexture;

	ACoreHudBase(const FObjectInitializer& ObjectInitializer);

	/** Icons for hit indicator. */
	UPROPERTY()
	FCanvasIcon HitNotifyIcon[8];

	/** Texture for hit indicator. */
	UPROPERTY()
	UTexture2D* HitNotifyTexture;

	UFUNCTION(BlueprintCallable, Category = "CFG")
	EHUDState GetCurrentState();

	/* An event hook to call HUD text events to display in the HUD. Blueprint HUD class must implement how to deal with this event. */
	UFUNCTION(BlueprintImplementableEvent, Category = "CFG")
	void MessageReceived(const FText& TextMessage);

	/* Event hook to update HUD state (eg. to determine visibility of widgets) */
	UFUNCTION(BlueprintNativeEvent, Category = "CFG")
	void OnStateChanged(EHUDState NewState);

	UFUNCTION(BlueprintCallable, Category = "CFG")
	void AddHudComponent(FString Name, UCoreHudComponent* CoreHudComponent);

	UFUNCTION(BlueprintCallable, Category = "CFG")
	void RemoveHudComponent(FString Name);

	UFUNCTION(BlueprintCallable, Category = "CFG")
	UCoreHudComponent* GetHudComponent(FString Name);

	UFUNCTION(BlueprintCallable, Category = "CFG")
	TArray<UCoreHudComponent*> GetAllHudComponents();
	
	UFUNCTION(BlueprintCallable, Category = "CFG")
	void SetMessageVisible(bool bVisible);

	UFUNCTION(BlueprintCallable, Category = "CFG")
	void SetMessageText(const FString& Message);

	UPROPERTY()
	UFont* NormalFont;

	UFUNCTION(BlueprintImplementableEvent, Category = "CFG")
	void UpdateAmmoUseEvent();

private:
	FCanvasIcon CenterDotIcon;

	
	EHUDState CurrentState;				/* Current HUD state */
	float LastEnemyHitTime;				/** When we last time hit the enemy. */
	float LastEnemyHitDisplayTime;		/** How long till enemy hit notify fades out completely. */
	float HitNotifyDisplayTime;			/** How long till hit notify fades out completely. */
	float LastHitTime;					/** Most recent hit time, used to check if we need to draw hit indicator at all. */
	FVector2D Offsets[8];				/** Offsets to display hit indicator parts. */
	FHitData HitNotifyData[8];			/** Runtime data for hit indicator. */

	bool  bPickupMessageVisible;
	float ScaleUI;						/** UI scaling factor for other resolutions than Full HD. */
	FFontRenderInfo ShadowedFont;		/** FontRenderInfo enabling casting shadow.s */
	static const float MinHudScale;		/** Floor for automatic hud scaling. */

	void DrawMessage();
	void DrawCrosshair();
	void DrawHitIndicator();

	TMap<FString, UCoreHudComponent*> CoreHudComponents;

	FVector Dirs2[8] = {
		FVector(0,-1,0) /*left*/,
		FVector(1,-1,0) /*front left*/,
		FVector(1,0,0) /*front*/,
		FVector(1,1,0) /*front right*/,
		FVector(0,1,0) /*right*/,
		FVector(-1,1,0) /*back right*/,
		FVector(-1,0,0), /*back*/
		FVector(-1,-1,0) /*back left*/
	};

};