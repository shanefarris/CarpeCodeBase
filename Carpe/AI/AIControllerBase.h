// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "AIController.h"
#include "BotWaypoint.h"
#include "AIControllerBase.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;

class ABotWaypoint;

UCLASS(config=Game)
class AAIControllerBase : public AAIController
{
	GENERATED_UCLASS_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "CFG|AI")
	FName TargetEnemyKeyName;

	UPROPERTY(EditAnywhere, Category = "CFG|AI")
	FName PatrolLocationKeyName;

	UPROPERTY(EditAnywhere, Category = "CFG|AI")
	FName CurrentWaypointKeyName;

	UPROPERTY(EditAnywhere, Category = "CFG|AI")
	FName BotTypeKeyName;

	/* Cached BT component */
	UPROPERTY(transient)
	UBehaviorTreeComponent* BehaviorComp;

protected:
	// Check of we have LOS to a character
	bool LOSTrace(ACharacterBase* InEnemyChar) const;

	int32 EnemyKeyID;
	int32 NeedAmmoKeyID;

	/** Handle for efficient management of Respawn timer */
	FTimerHandle TimerHandle_Respawn;

public:

	// Begin AController interface
	virtual void GameHasEnded(class AActor* EndGameFocus = NULL, bool bIsWinner = false) override;

	virtual void Possess(class APawn* InPawn) override;
	virtual void UnPossess() override;

	virtual void BeginInactiveState() override;
	// End APlayerController interface

	void Respawn();

	void CheckAmmo(const class AWeapon* CurrentWeapon);

	void SetEnemy(class APawn* InPawn);

	class ACharacterBase* GetEnemy() const;

	/* If there is line of sight to current enemy, start firing at them */
	UFUNCTION(BlueprintCallable, Category= "CFG|Behavior")
	void ShootEnemy();

	/* Finds the closest enemy and sets them as current target */
	UFUNCTION(BlueprintCallable, Category= "CFG|Behavior")
	void FindClosestEnemy();

	UFUNCTION(BlueprintCallable, Category = "CFG|Behavior")
	bool FindClosestEnemyWithLOS(ACharacterBase* ExcludeEnemy);
		
	bool HasWeaponLOSToEnemy(AActor* InEnemyActor, const bool bAnyEnemy) const;

	// Begin AAIController interface
	/** Update direction AI is looking based on FocalPoint */
	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true) override;
	// End AAIController interface

	ABotWaypoint* GetWaypoint();
	void SetWaypoint(ABotWaypoint* NewWaypoint);

public:
	/** Returns BlackboardComp subobject **/
	FORCEINLINE UBlackboardComponent* GetBlackboard() const { return Blackboard; }
	/** Returns BehaviorComp subobject **/
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }
};
