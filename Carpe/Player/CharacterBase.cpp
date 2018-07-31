#include "CharacterBase.h"
#include "Carpe.h"
#include "Types.h"
#include "Inventory/Weapon.h"
#include "Inventory/GameDamageType.h"
#include "Inventory/InventoryItem.h"
#include "UI/CoreHudBase.h"
#include "Player/GamePlayerState.h"
#include "Player/CharacterMovement.h"

#include "Runtime/Engine/Classes/Animation/AnimMontage.h"

ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovement>(ACharacter::CharacterMovementComponentName))
{
	Mesh1P = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("PawnMesh1P"));
	Mesh1P->SetupAttachment(GetCapsuleComponent());
	Mesh1P->bOnlyOwnerSee = true;
	Mesh1P->bOwnerNoSee = false;
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->bReceivesDecals = false;
	Mesh1P->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	Mesh1P->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Mesh1P->SetCollisionObjectType(ECC_Pawn);
	Mesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh1P->SetCollisionResponseToAllChannels(ECR_Ignore);

	// 3rd person camera
	//ThirdPersonCameraArm = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("ThirdPersonCameraArm"));
	//ThirdPersonCameraArm->TargetOffset = FVector(0.f, 0.f, 0.f);
	//ThirdPersonCameraArm->SetRelativeLocation(FVector(-40.f, 0.f, 160.f));
	//ThirdPersonCameraArm->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));
	//ThirdPersonCameraArm->AttachTo(GetMesh()); // attach it to the third person mesh
	//ThirdPersonCameraArm->TargetArmLength = 200.f;
	//ThirdPersonCameraArm->bEnableCameraLag = false;
	//ThirdPersonCameraArm->bEnableCameraRotationLag = false;
	//ThirdPersonCameraArm->bUsePawnControlRotation = true; // let the controller handle the view rotation
	//ThirdPersonCameraArm->bInheritYaw = true;
	//ThirdPersonCameraArm->bInheritPitch = true;
	//ThirdPersonCameraArm->bInheritRoll = false;

	//ThirdPersonCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("ThirdPersonCamera"));
	//ThirdPersonCamera->AttachTo(ThirdPersonCameraArm, USpringArmComponent::SocketName);
	//ThirdPersonCamera->bUsePawnControlRotation = false; // the arm is already doing the rotation
	//ThirdPersonCamera->FieldOfView = 90.f;

	GetMesh()->bOnlyOwnerSee = false;
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->bReceivesDecals = false;
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(COLLISION_PROJECTILE2, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PROJECTILE2, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	TargetingSpeedModifier = 0.5f;
	bIsTargeting = false;
	RunningSpeedModifier = 1.5f;
	bWantsToRun = false;
	bWantsToFire = false;
	bIsThirdPerson = false;
	LowHealthPercentage = 0.5f;
	MaxUseDistance = 1000.0f;
	bIsReloading = false;
	ReloadSpeed = 1.0f;
	bIsDying = false;
	MaxWeaponsCanCarry = 2;

	FocusedInventoryItem = nullptr;

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	SetThirdPerson(false);
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Role == ROLE_Authority)
	{
		Health = GetMaxHealth();
		SpawnDefaultInventory();
	}

	// set initial mesh visibility (3rd person view)
	UpdatePawnMeshes();

	// create material instance for setting team colors (3rd person view)
	for (int32 iMat = 0; iMat < GetMesh()->GetNumMaterials(); iMat++)
	{
		MeshMIDs.Add(GetMesh()->CreateAndSetMaterialInstanceDynamic(iMat));
	}

	// play respawn effects
	if (GetNetMode() != NM_DedicatedServer)
	{
		if (RespawnFX)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, RespawnFX, GetActorLocation(), GetActorRotation());
		}

		if (RespawnSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, RespawnSound, GetActorLocation());
		}
	}
}

void ACharacterBase::Destroyed()
{
	Super::Destroyed();
	DestroyInventory();
}

void ACharacterBase::PawnClientRestart()
{
	Super::PawnClientRestart();

	// switch mesh to 1st person view
	UpdatePawnMeshes();

	// reattach weapon if needed
	SetCurrentWeapon(CurrentWeapon);

	// set team colors for 1st person view
	UMaterialInstanceDynamic* Mesh1PMID = Mesh1P->CreateAndSetMaterialInstanceDynamic(0);
	UpdateTeamColors(Mesh1PMID);
}

void ACharacterBase::PossessedBy(class AController* InController)
{
	Super::PossessedBy(InController);

	// [server] as soon as PlayerState is assigned, set team colors of this pawn for local player
	UpdateTeamColorsAllMIDs();
}

void ACharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// [client] as soon as PlayerState is assigned, set team colors of this pawn for local player
	if (PlayerState != NULL)
	{
		UpdateTeamColorsAllMIDs();
	}
}

FRotator ACharacterBase::GetAimOffsets() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}

bool ACharacterBase::IsEnemyFor(AController* TestPC) const
{
	if (TestPC == Controller || TestPC == nullptr)
	{
		return false;
	}

	auto TestPlayerState = Cast<AGamePlayerState>(TestPC->PlayerState);
	auto MyPlayerState = Cast<AGamePlayerState>(PlayerState);

	bool bIsEnemy = true;
	if (GetWorld()->GetGameState() && GetWorld()->GetGameState()->GameModeClass)
	{
		auto DefGame = GetWorld()->GetGameState()->GameModeClass->GetDefaultObject<ACoreGameModeBase>();
		if (DefGame && MyPlayerState && TestPlayerState)
		{
			bIsEnemy = DefGame->CanDealDamage(TestPlayerState, MyPlayerState);
		}
	}

	return bIsEnemy;
}

//////////////////////////////////////////////////////////////////////////
// Meshes
void ACharacterBase::UpdatePawnMeshes()
{
	bool const bFirstPerson = IsFirstPerson();

	bFindCameraComponentWhenViewTarget = !bFirstPerson;
	Mesh1P->MeshComponentUpdateFlag = !bFirstPerson ? EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered : EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
	Mesh1P->SetOwnerNoSee(!bFirstPerson);

	GetMesh()->MeshComponentUpdateFlag = bFirstPerson ? EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered : EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetOwnerNoSee(bFirstPerson);

	/*if (Controller)
	{
	Controller->bFindCameraComponentWhenViewTarget = false;
	}*/

	if (CurrentWeapon)
	{
		CurrentWeapon->UpdateMeshes();
	}
}

void ACharacterBase::UpdateTeamColors(UMaterialInstanceDynamic* UseMID)
{
	if (UseMID)
	{
		auto MyPlayerState = Cast<AGamePlayerState>(PlayerState);
		if (MyPlayerState != NULL)
		{
			float MaterialParam = (float)MyPlayerState->GetTeamNum();
			UseMID->SetScalarParameterValue(TEXT("Team Color Index"), MaterialParam);
		}
	}
}

void ACharacterBase::OnCameraUpdate(const FVector& CameraLocation, const FRotator& CameraRotation)
{
	if (IsFirstPerson())
	{
		UpdateCameraFirstPerson(CameraLocation, CameraRotation);
	}
	else
	{
		UpdateCameraThirdPerson(CameraLocation, CameraRotation);
	}
}

void ACharacterBase::UpdateCameraFirstPerson(const FVector& CameraLocation, const FRotator& CameraRotation)
{
	USkeletalMeshComponent* DefMesh1P = Cast<USkeletalMeshComponent>(GetClass()->GetDefaultSubobjectByName(TEXT("PawnMesh1P")));
	const FMatrix DefMeshLS = FRotationTranslationMatrix(DefMesh1P->RelativeRotation, DefMesh1P->RelativeLocation);
	const FMatrix LocalToWorld = ActorToWorld().ToMatrixWithScale();

	// Mesh rotating code expect uniform scale in LocalToWorld matrix

	const FRotator RotCameraPitch(CameraRotation.Pitch, 0.0f, 0.0f);
	const FRotator RotCameraYaw(0.0f, CameraRotation.Yaw, 0.0f);

	const FMatrix LeveledCameraLS = FRotationTranslationMatrix(RotCameraYaw, CameraLocation) * LocalToWorld.Inverse();
	const FMatrix PitchedCameraLS = FRotationMatrix(RotCameraPitch) * LeveledCameraLS;
	const FMatrix MeshRelativeToCamera = DefMeshLS * LeveledCameraLS.Inverse();
	const FMatrix PitchedMesh = MeshRelativeToCamera * PitchedCameraLS;

	Mesh1P->SetRelativeLocationAndRotation(PitchedMesh.GetOrigin(), PitchedMesh.Rotator());
}

void ACharacterBase::UpdateCameraThirdPerson(const FVector& CameraLocation, const FRotator& CameraRotation)
{
	if (Controller && ThirdPersonCamera)
	{
		ThirdPersonCamera->FieldOfView = Cast<APlayerController>(Controller)->PlayerCameraManager->DefaultFOV;
	}
}

//////////////////////////////////////////////////////////////////////////
// Damage & death
void ACharacterBase::FellOutOfWorld(const class UDamageType& dmgType)
{
	Die(Health, FDamageEvent(dmgType.GetClass()), NULL, NULL);
}

void ACharacterBase::Suicide()
{
	KilledBy(this);
}

void ACharacterBase::KilledBy(APawn* EventInstigator)
{
	if (Role == ROLE_Authority && !bIsDying)
	{
		AController* Killer = NULL;
		if (EventInstigator != NULL)
		{
			Killer = EventInstigator->Controller;
			LastHitBy = NULL;
		}

		Die(Health, FDamageEvent(UDamageType::StaticClass()), Killer, NULL);
	}
}

float ACharacterBase::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	auto MyPC = Cast<AGamePlayerController>(Controller);
	if (MyPC && MyPC->HasGodMode())
	{
		return 0.f;
	}

	if (Health <= 0.f)
	{
		return 0.f;
	}

	// Modify based on game rules.
	auto const Game = GetWorld()->GetAuthGameMode<ACoreGameModeBase>();
	Damage = Game ? Game->ModifyDamage(Damage, this, DamageEvent, EventInstigator, DamageCauser) : 0.f;

	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		Health -= ActualDamage;
		if (Health <= 0)
		{
			Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
		}
		else
		{
			PlayHit(ActualDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : nullptr, DamageCauser);
		}

		MakeNoise(1.0f, EventInstigator ? EventInstigator->GetPawn() : this);
	}

	return ActualDamage;
}

bool ACharacterBase::CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const
{
	auto gameMode = GetWorld()->GetAuthGameMode<ACoreGameModeBase>();

	if (bIsDying ||									// already dying
		IsPendingKill() ||							// already destroyed
		Role != ROLE_Authority ||					// not authority
		gameMode == nullptr || gameMode->GetMatchState() == MatchState::LeavingMap)	// level transition occurring
	{
		return false;
	}

	return true;
}

bool ACharacterBase::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	if (!CanDie(KillingDamage, DamageEvent, Killer, DamageCauser))
	{
		return false;
	}

	Health = FMath::Min(0.0f, Health);

	// if this is an environmental death then refer to the previous killer so that they receive credit (knocked into lava pits, etc)
	auto DamageType = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	Killer = GetDamageInstigator(Killer, *DamageType);

	AController* const KilledPlayer = (Controller != nullptr) ? Controller : Cast<AController>(GetOwner());
	GetWorld()->GetAuthGameMode<ACoreGameModeBase>()->Killed(Killer, KilledPlayer, this, DamageType);

	NetUpdateFrequency = GetDefault<ACharacterBase>()->NetUpdateFrequency;
	GetCharacterMovement()->ForceReplicationUpdate();

	OnDeath(KillingDamage, DamageEvent, Killer ? Killer->GetPawn() : nullptr, DamageCauser);
	return true;
}

void ACharacterBase::OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	if (bIsDying)
	{
		return;
	}

	bReplicateMovement = false;
	bTearOff = true;
	bIsDying = true;

	if (Role == ROLE_Authority)
	{
		ReplicateHit(KillingDamage, DamageEvent, PawnInstigator, DamageCauser, true);
	}

	// cannot use IsLocallyControlled here, because even local client's controller may be NULL here
	if (GetNetMode() != NM_DedicatedServer && DeathSound && Mesh1P && Mesh1P->IsVisible())
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	// remove all weapons
	DestroyInventory();

	// switch back to 3rd person view
	UpdatePawnMeshes();

	DetachFromControllerPendingDestroy();
	StopAllAnimMontages();

	if (LowHealthWarningPlayer && LowHealthWarningPlayer->IsPlaying())
	{
		LowHealthWarningPlayer->Stop();
	}

	if (RunLoopAC)
	{
		RunLoopAC->Stop();
	}



	if (GetMesh())
	{
		static FName CollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetCollisionProfileName(CollisionProfileName);
	}
	SetActorEnableCollision(true);

	// Death anim
	//StopAllAnimMontages();
	float DeathAnimDuration = PlayAnimMontage(DeathAnim);

	// Ragdoll
	FTimerHandle TimerHandle;		// Use a local timer handle as we don't need to store it for later but we don't need to look for something to clear
	if (RagdollOnDeath)
	{
		if (DeathAnimDuration > 0.f)
		{
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ACharacterBase::SetRagdollPhysics, FMath::Min(0.1f, DeathAnimDuration), false);
		}
		else
		{
			SetRagdollPhysics();
		}
	}
	else
	{
		bPlayDeathAnim = true;
		//GetMesh()->bNoSkeletonUpdate = 1;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ACharacterBase::TurnOff, DeathAnimDuration, false);
	}

	// disable collisions on capsule
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void ACharacterBase::TurnOff()
{
	SetActorHiddenInGame(true);
	SetLifeSpan(0.1f);

	Super::TurnOff();
}

void ACharacterBase::PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	if (Role == ROLE_Authority)
	{
		ReplicateHit(DamageTaken, DamageEvent, PawnInstigator, DamageCauser, false);

		// play the force feedback effect on the client player controller
		APlayerController* PC = Cast<APlayerController>(Controller);
		if (PC && DamageEvent.DamageTypeClass)
		{
			auto DamageType = Cast<UDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject());
			//if (DamageType && DamageType->HitForceFeedback)
			//{
			//	PC->ClientPlayForceFeedback(DamageType->HitForceFeedback, false, "Damage");
			//}
		}
	}

	if (DamageTaken > 0.f)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	}

	APlayerController* MyPC = Cast<APlayerController>(Controller);

	auto MyHUD = MyPC ? Cast<ACoreHudBase>(MyPC->GetHUD()) : nullptr;
	if (MyHUD)
	{
		MyHUD->NotifyWeaponHit(DamageTaken, DamageEvent, PawnInstigator);
	}

	if (PawnInstigator && PawnInstigator != this && PawnInstigator->IsLocallyControlled())
	{
		auto InstigatorPC = Cast<APlayerController>(PawnInstigator->Controller);
		auto InstigatorHUD = InstigatorPC ? Cast<ACoreHudBase>(InstigatorPC->GetHUD()) : nullptr;
		if (InstigatorHUD)
		{
			InstigatorHUD->NotifyEnemyHit();
		}
	}
}

void ACharacterBase::SetRagdollPhysics()
{
	bool bInRagdoll = false;

	if (IsPendingKill())
	{
		bInRagdoll = false;
	}
	else if (!GetMesh() || !GetMesh()->GetPhysicsAsset())
	{
		bInRagdoll = false;
	}
	else
	{
		// initialize physics/etc
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->bBlendPhysics = true;

		bInRagdoll = true;
	}

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);

	if (!bInRagdoll)
	{
		// hide and set short lifespan
		TurnOff();
		SetActorHiddenInGame(true);
		SetLifeSpan(1.0f);
	}
	else
	{
		SetLifeSpan(10.0f);
	}
}

void ACharacterBase::ReplicateHit(float Damage, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser, bool bKilled)
{
	const float TimeoutTime = GetWorld()->GetTimeSeconds() + 0.5f;

	FDamageEvent const& LastDamageEvent = LastTakeHitInfo.GetDamageEvent();
	if ((PawnInstigator == LastTakeHitInfo.PawnInstigator.Get()) && (LastDamageEvent.DamageTypeClass == LastTakeHitInfo.DamageTypeClass) && (LastTakeHitTimeTimeout == TimeoutTime))
	{
		// same frame damage
		if (bKilled && LastTakeHitInfo.bKilled)
		{
			// Redundant death take hit, just ignore it
			return;
		}

		// otherwise, accumulate damage done this frame
		Damage += LastTakeHitInfo.ActualDamage;
	}

	LastTakeHitInfo.ActualDamage = Damage;
	LastTakeHitInfo.PawnInstigator = Cast<ACharacterBase>(PawnInstigator);
	LastTakeHitInfo.DamageCauser = DamageCauser;
	LastTakeHitInfo.SetDamageEvent(DamageEvent);
	LastTakeHitInfo.bKilled = bKilled;
	LastTakeHitInfo.EnsureReplication();

	LastTakeHitTimeTimeout = TimeoutTime;
}

void ACharacterBase::OnRep_LastTakeHitInfo()
{
	if (LastTakeHitInfo.bKilled)
	{
		OnDeath(LastTakeHitInfo.ActualDamage, LastTakeHitInfo.GetDamageEvent(), LastTakeHitInfo.PawnInstigator.Get(), LastTakeHitInfo.DamageCauser.Get());
	}
	else
	{
		PlayHit(LastTakeHitInfo.ActualDamage, LastTakeHitInfo.GetDamageEvent(), LastTakeHitInfo.PawnInstigator.Get(), LastTakeHitInfo.DamageCauser.Get());
	}
}

//Pawn::PlayDying sets this lifespan, but when that function is called on client, dead pawn's role is still SimulatedProxy despite bTearOff being true. 
void ACharacterBase::TornOff()
{
	SetLifeSpan(25.f);
}

//////////////////////////////////////////////////////////////////////////
// Inventory
void ACharacterBase::SpawnDefaultInventory()
{
	if (Role < ROLE_Authority)
	{
		return;
	}

	int32 NumWeaponClasses = DefaultInventoryClasses.Num();
	for (int32 i = 0; i < NumWeaponClasses; i++)
	{
		if (DefaultInventoryClasses[i])
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultInventoryClasses[i], SpawnInfo);
			AddWeapon(NewWeapon);
		}
	}

	// equip first weapon in inventory
	if (Inventory.Num() > 0)
	{
		EquipWeapon(Inventory[0]);
	}
}

void ACharacterBase::DestroyInventory()
{
	if (Role < ROLE_Authority)
	{
		return;
	}

	// remove all weapons from inventory and destroy them
	for (int32 i = Inventory.Num() - 1; i >= 0; i--)
	{
		AWeapon* Weapon = Inventory[i];
		if (Weapon)
		{
			RemoveWeapon(Weapon);
			Weapon->Destroy();
		}
	}
}

void ACharacterBase::DropWeapon(AWeapon* Weapon)
{
	if (Weapon)
	{
		if (Role < ROLE_Authority)
		{
			return;
		}

		// remove all weapons from inventory and destroy them
		for (int32 i = Inventory.Num() - 1; i >= 0; i--)
		{
			if (Weapon == Inventory[i])
			{
				RemoveWeapon(Weapon);
				Weapon->Destroy();
			}
		}
	}
}

void ACharacterBase::AddWeapon(AWeapon* Weapon)
{
	if (Weapon && Role == ROLE_Authority)
	{
		// If we do not have the max number of weapons to carry, just add it
		if (Inventory.Num() >= MaxWeaponsCanCarry)
		{
			DropWeapon(CurrentWeapon);
		}

		Weapon->OnEnterInventory(this);
		Inventory.AddUnique(Weapon);
	}
}

void ACharacterBase::RemoveWeapon(AWeapon* Weapon)
{
	if (Weapon && Role == ROLE_Authority)
	{
		Weapon->OnLeaveInventory();
		Inventory.RemoveSingle(Weapon);
	}
}

AWeapon* ACharacterBase::FindWeapon(TSubclassOf<AWeapon> WeaponClass)
{
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i] && Inventory[i]->IsA(WeaponClass))
		{
			return Inventory[i];
		}
	}

	return nullptr;
}

void ACharacterBase::EquipWeapon(AWeapon* Weapon)
{
	if (Weapon)
	{
		if (Role == ROLE_Authority)
		{
			SetCurrentWeapon(Weapon, CurrentWeapon);
		}
		else
		{
			ServerEquipWeapon(Weapon);
		}
	}
}

bool ACharacterBase::ServerEquipWeapon_Validate(AWeapon* Weapon)
{
	return true;
}

void ACharacterBase::ServerEquipWeapon_Implementation(AWeapon* Weapon)
{
	EquipWeapon(Weapon);
}

void ACharacterBase::OnRep_CurrentWeapon(AWeapon* LastWeapon)
{
	SetCurrentWeapon(CurrentWeapon, LastWeapon);
}

void ACharacterBase::SetCurrentWeapon(AWeapon* NewWeapon, AWeapon* LastWeapon)
{
	AWeapon* LocalLastWeapon = nullptr;

	if (LastWeapon != nullptr)
	{
		LocalLastWeapon = LastWeapon;
	}
	else if (NewWeapon != CurrentWeapon)
	{
		LocalLastWeapon = CurrentWeapon;
	}

	// unequip previous
	if (LocalLastWeapon)
	{
		LocalLastWeapon->OnUnEquip();
	}

	CurrentWeapon = NewWeapon;

	// equip new one
	if (NewWeapon)
	{
		NewWeapon->SetOwningPawn(this);	// Make sure weapon's MyPawn is pointing back to us. During replication, we can't guarantee APawn::CurrentWeapon will rep after AWeapon::MyPawn!
		NewWeapon->OnEquip(LastWeapon);
	}
}


//////////////////////////////////////////////////////////////////////////
// Weapon usage
void ACharacterBase::StartWeaponFire()
{
	if (!bWantsToFire)
	{
		bWantsToFire = true;
		if (CurrentWeapon)
		{
			CurrentWeapon->StartFire();
		}
	}
}

void ACharacterBase::StopWeaponFire()
{
	if (bWantsToFire)
	{
		bWantsToFire = false;
		if (CurrentWeapon)
		{
			CurrentWeapon->StopFire();
		}
	}
}

bool ACharacterBase::CanFire() const
{
	return CurrentWeapon && IsAlive();
}

bool ACharacterBase::CanReload() const
{
	return true;
}

void ACharacterBase::SetTargeting(bool bNewTargeting)
{
	bIsTargeting = bNewTargeting;

	if (TargetingSound)
	{
		UGameplayStatics::SpawnSoundAttached(TargetingSound, GetRootComponent());
	}

	if (Role < ROLE_Authority)
	{
		ServerSetTargeting(bNewTargeting);
	}
}

bool ACharacterBase::ServerSetTargeting_Validate(bool bNewTargeting)
{
	return true;
}

void ACharacterBase::ServerSetTargeting_Implementation(bool bNewTargeting)
{
	SetTargeting(bNewTargeting);
}

//////////////////////////////////////////////////////////////////////////
// Movement
void ACharacterBase::SetRunning(bool bNewRunning, bool bToggle)
{
	bWantsToRun = bNewRunning;
	bWantsToRunToggled = bNewRunning && bToggle;

	if (Role < ROLE_Authority)
	{
		ServerSetRunning(bNewRunning, bToggle);
	}

	UpdateRunSounds(bNewRunning);
}

bool ACharacterBase::ServerSetRunning_Validate(bool bNewRunning, bool bToggle)
{
	return true;
}

void ACharacterBase::ServerSetRunning_Implementation(bool bNewRunning, bool bToggle)
{
	SetRunning(bNewRunning, bToggle);
}

void ACharacterBase::UpdateRunSounds(bool bNewRunning)
{
	if (bNewRunning)
	{
		if (!RunLoopAC && RunLoopSound)
		{
			RunLoopAC = UGameplayStatics::SpawnSoundAttached(RunLoopSound, GetRootComponent());
			if (RunLoopAC)
			{
				RunLoopAC->bAutoDestroy = false;
			}

		}
		else if (RunLoopAC)
		{
			RunLoopAC->Play();
		}
	}
	else
	{
		if (RunLoopAC)
		{
			RunLoopAC->Stop();
		}

		if (RunStopSound)
		{
			UGameplayStatics::SpawnSoundAttached(RunStopSound, GetRootComponent());
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Animations
float ACharacterBase::PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	USkeletalMeshComponent* UseMesh = GetMesh();
	if (AnimMontage && UseMesh && UseMesh->AnimScriptInstance)
	{
		return UseMesh->AnimScriptInstance->Montage_Play(AnimMontage, InPlayRate);
	}

	return 0.0f;
}

void ACharacterBase::StopAnimMontage(class UAnimMontage* AnimMontage)
{
	USkeletalMeshComponent* UseMesh = GetPawnMesh();
	if (AnimMontage && UseMesh && UseMesh->AnimScriptInstance &&
		UseMesh->AnimScriptInstance->Montage_IsPlaying(AnimMontage))
	{
		UseMesh->AnimScriptInstance->Montage_Stop(AnimMontage->BlendOut.GetBlendTime(), AnimMontage);
	}
}

void ACharacterBase::StopAllAnimMontages()
{
	USkeletalMeshComponent* UseMesh = GetPawnMesh();
	if (UseMesh && UseMesh->AnimScriptInstance)
	{
		UseMesh->AnimScriptInstance->Montage_Stop(0.0f);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input
void ACharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ACharacterBase::MoveUp);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACharacterBase::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACharacterBase::LookUpAtRate);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACharacterBase::OnStartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACharacterBase::OnStopFire);

	PlayerInputComponent->BindAction("Targeting", IE_Pressed, this, &ACharacterBase::OnStartTargeting);
	PlayerInputComponent->BindAction("Targeting", IE_Released, this, &ACharacterBase::OnStopTargeting);

	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this, &ACharacterBase::OnNextWeapon);
	PlayerInputComponent->BindAction("PrevWeapon", IE_Pressed, this, &ACharacterBase::OnPrevWeapon);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ACharacterBase::OnReload);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterBase::OnStartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacterBase::OnStopJump);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ACharacterBase::OnStartRunning);
	PlayerInputComponent->BindAction("RunToggle", IE_Pressed, this, &ACharacterBase::OnStartRunningToggle);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ACharacterBase::OnStopRunning);

	PlayerInputComponent->BindAction("Action", IE_Released, this, &ACharacterBase::OnAction);

	// 3rd person
	PlayerInputComponent->BindAction("ThirdPerson", IE_Pressed, this, &ACharacterBase::OnThirdPerson);
	PlayerInputComponent->BindAction("ThirdPersonToggle", IE_Pressed, this, &ACharacterBase::OnThirdPersonToggle);
	PlayerInputComponent->BindAction("ThirdPerson", IE_Released, this, &ACharacterBase::OnFirstPerson);
}

void ACharacterBase::MoveForward(float Val)
{
	if (Controller && Val != 0.f)
	{
		// Limit pitch when walking or falling
		const bool bLimitRotation = (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling());
		const FRotator Rotation = bLimitRotation ? GetActorRotation() : Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Val);
	}
}

void ACharacterBase::MoveRight(float Val)
{
	if (Val != 0.f)
	{
		const FQuat Rotation = GetActorQuat();
		const FVector Direction = FQuatRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Val);
	}
}

void ACharacterBase::MoveUp(float Val)
{
	if (Val != 0.f)
	{
		// Not when walking or falling.
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
		{
			return;
		}

		AddMovementInput(FVector::UpVector, Val);
	}
}

void ACharacterBase::TurnAtRate(float Val)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Val * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::LookUpAtRate(float Val)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Val * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::OnStartFire()
{
	auto MyPC = Cast<AGamePlayerController>(Controller);
	if (MyPC && MyPC->IsGameInputAllowed())
	{
		if (IsRunning())
		{
			SetRunning(false, false);
		}
		StartWeaponFire();
	}
}

void ACharacterBase::OnStopFire()
{
	StopWeaponFire();
}

void ACharacterBase::OnStartTargeting()
{
	auto MyPC = Cast<AGamePlayerController>(Controller);
	if (MyPC && MyPC->IsGameInputAllowed())
	{
		if (IsRunning())
		{
			SetRunning(false, false);
		}
		SetTargeting(true);
	}
}

void ACharacterBase::OnStopTargeting()
{
	SetTargeting(false);
}

void ACharacterBase::OnNextWeapon()
{
	auto MyPC = Cast<AGamePlayerController>(Controller);
	if (MyPC && MyPC->IsGameInputAllowed())
	{
		if (Inventory.Num() >= 2 && (CurrentWeapon == NULL || CurrentWeapon->GetCurrentState() != EInventoryState::Equipping))
		{
			const int32 CurrentWeaponIdx = Inventory.IndexOfByKey(CurrentWeapon);
			AWeapon* NextWeapon = Inventory[(CurrentWeaponIdx + 1) % Inventory.Num()];
			EquipWeapon(NextWeapon);
		}
	}
}

void ACharacterBase::OnPrevWeapon()
{
	auto MyPC = Cast<AGamePlayerController>(Controller);
	if (MyPC && MyPC->IsGameInputAllowed())
	{
		if (Inventory.Num() >= 2 && (CurrentWeapon == NULL || CurrentWeapon->GetCurrentState() != EInventoryState::Equipping))
		{
			const int32 CurrentWeaponIdx = Inventory.IndexOfByKey(CurrentWeapon);
			AWeapon* PrevWeapon = Inventory[(CurrentWeaponIdx - 1 + Inventory.Num()) % Inventory.Num()];
			EquipWeapon(PrevWeapon);
		}
	}
}

void ACharacterBase::OnReload()
{
	auto MyPC = Cast<AGamePlayerController>(Controller);
	if (MyPC && MyPC->IsGameInputAllowed())
	{
		if (CurrentWeapon)
		{
			CurrentWeapon->StartReload();
		}
	}
}

void ACharacterBase::OnStartRunning()
{
	auto MyPC = Cast<AGamePlayerController>(Controller);
	if (MyPC && MyPC->IsGameInputAllowed())
	{
		if (IsTargeting())
		{
			SetTargeting(false);
		}
		StopWeaponFire();
		SetRunning(true, false);
	}
}

void ACharacterBase::OnStartRunningToggle()
{
	auto MyPC = Cast<AGamePlayerController>(Controller);
	if (MyPC && MyPC->IsGameInputAllowed())
	{
		if (IsTargeting())
		{
			SetTargeting(false);
		}
		StopWeaponFire();
		SetRunning(true, true);
	}
}

void ACharacterBase::OnStopRunning()
{
	SetRunning(false, false);
}

void ACharacterBase::OnAction()
{
	if (FocusedInventoryItem)
	{
		if (FocusedInventoryItem->CanBePickedUp(this))
		{
			FocusedInventoryItem->GivePickupTo(this);
		}
	}
}

bool ACharacterBase::IsRunning() const
{
	if (!GetCharacterMovement())
	{
		return false;
	}

	return (bWantsToRun || bWantsToRunToggled) && !GetVelocity().IsZero() && (GetVelocity().GetSafeNormal2D() | GetActorForwardVector()) > -0.1;
}

void ACharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bWantsToRunToggled && !IsRunning())
	{
		SetRunning(false, false);
	}

	auto MyPC = Cast<AGamePlayerController>(Controller);
	if (MyPC && MyPC->HasHealthRegen())
	{
		if (this->Health < this->GetMaxHealth())
		{
			this->Health += 5 * DeltaSeconds;
			if (Health > this->GetMaxHealth())
			{
				Health = this->GetMaxHealth();
			}
		}
	}

	if (LowHealthSound && GEngine->UseSound())
	{
		if ((this->Health > 0 && this->Health < this->GetMaxHealth() * LowHealthPercentage) && (!LowHealthWarningPlayer || !LowHealthWarningPlayer->IsPlaying()))
		{
			LowHealthWarningPlayer = UGameplayStatics::SpawnSoundAttached(LowHealthSound, GetRootComponent(),
				NAME_None, FVector(ForceInit), EAttachLocation::KeepRelativeOffset, true);
			LowHealthWarningPlayer->SetVolumeMultiplier(0.0f);
		}
		else if ((this->Health > this->GetMaxHealth() * LowHealthPercentage || this->Health < 0) && LowHealthWarningPlayer && LowHealthWarningPlayer->IsPlaying())
		{
			LowHealthWarningPlayer->Stop();
		}
		if (LowHealthWarningPlayer && LowHealthWarningPlayer->IsPlaying())
		{
			const float MinVolume = 0.3f;
			const float VolumeMultiplier = (1.0f - (this->Health / (this->GetMaxHealth() * LowHealthPercentage)));
			LowHealthWarningPlayer->SetVolumeMultiplier(MinVolume + (1.0f - MinVolume) * VolumeMultiplier);
		}
	}

	// Perform ray-trace to find closest looked-at UsableActor.
	if (Controller != nullptr)
	{
		FVector CamLoc;
		FRotator CamRot;
		Controller->GetPlayerViewPoint(CamLoc, CamRot);
		const FVector TraceStart = CamLoc;
		const FVector Direction = CamRot.Vector();
		const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance);

		FCollisionQueryParams TraceParams(TEXT("TraceUsableActor"), true, this);
		TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = false;

		/* Not tracing complex uses the rough collision instead making tiny objects easier to select. */
		TraceParams.bTraceComplex = false;

		FHitResult Hit(ForceInit);
		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f);
		auto InventoryItem = Cast<AInventoryItem>(Hit.GetActor());
		if (FocusedInventoryItem && FocusedInventoryItem != InventoryItem)
		{
			FocusedInventoryItem->OnEndFocus();
		}

		if (InventoryItem)
		{
			FocusedInventoryItem = InventoryItem;
			FocusedInventoryItem->OnBeginFocus();
		}
	}

}

void ACharacterBase::OnStartJump()
{
	auto MyPC = Cast<AGamePlayerController>(Controller);
	if (MyPC && MyPC->IsGameInputAllowed())
	{
		bPressedJump = true;
		bIsReloading = false;

		if (CurrentWeapon)
		{
			CurrentWeapon->StopReload();
		}
	}
}

void ACharacterBase::OnStopJump()
{
	bPressedJump = false;
}

//////////////////////////////////////////////////////////////////////////
// Replication
void ACharacterBase::PreReplication(IRepChangedPropertyTracker & ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	// Only replicate this property for a short duration after it changes so join in progress players don't get spammed with fx when joining late
	DOREPLIFETIME_ACTIVE_OVERRIDE(ACharacterBase, LastTakeHitInfo, GetWorld() && GetWorld()->GetTimeSeconds() < LastTakeHitTimeTimeout);
}

void ACharacterBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// only to local owner: weapon change requests are locally instigated, other clients don't need it
	DOREPLIFETIME_CONDITION(ACharacterBase, Inventory, COND_OwnerOnly);

	// everyone except local owner: flag change is locally instigated
	DOREPLIFETIME_CONDITION(ACharacterBase, bIsTargeting, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ACharacterBase, bWantsToRun, COND_SkipOwner);

	DOREPLIFETIME_CONDITION(ACharacterBase, LastTakeHitInfo, COND_Custom);

	// 3rd person
	DOREPLIFETIME_CONDITION(ACharacterBase, bIsThirdPerson, COND_SkipOwner);

	// everyone
	DOREPLIFETIME(ACharacterBase, CurrentWeapon);
	DOREPLIFETIME(ACharacterBase, Health);
}

AWeapon* ACharacterBase::GetWeapon() const
{
	return CurrentWeapon;
}

int32 ACharacterBase::GetInventoryCount() const
{
	return Inventory.Num();
}

AWeapon* ACharacterBase::GetInventoryWeapon(int32 index) const
{
	return Inventory[index];
}

USkeletalMeshComponent* ACharacterBase::GetPawnMesh() const
{
	return IsFirstPerson() ? Mesh1P : GetMesh();
}

USkeletalMeshComponent* ACharacterBase::GetSpecifcPawnMesh(bool WantFirstPerson) const
{
	return WantFirstPerson == true ? Mesh1P : GetMesh();
}

FName ACharacterBase::GetWeaponAttachPoint() const
{
	return WeaponAttachPoint;
}

float ACharacterBase::GetTargetingSpeedModifier() const
{
	return TargetingSpeedModifier;
}

bool ACharacterBase::IsTargeting() const
{
	return bIsTargeting;
}

float ACharacterBase::GetRunningSpeedModifier() const
{
	return RunningSpeedModifier;
}

bool ACharacterBase::IsFiring() const
{
	return bWantsToFire;
};

bool ACharacterBase::IsFirstPerson() const
{
	return IsAlive() && Controller && Controller->IsLocalPlayerController() && !bIsThirdPerson;
}

int32 ACharacterBase::GetMaxHealth() const
{
	return GetClass()->GetDefaultObject<ACharacterBase>()->Health;
}

bool ACharacterBase::IsAlive() const
{
	return Health > 0;
}

float ACharacterBase::GetLowHealthPercentage() const
{
	return LowHealthPercentage;
}

void ACharacterBase::UpdateTeamColorsAllMIDs()
{
	for (int32 i = 0; i < MeshMIDs.Num(); ++i)
	{
		UpdateTeamColors(MeshMIDs[i]);
	}
}

void ACharacterBase::SetThirdPerson(bool bNewThirdPerson)
{
	bIsThirdPerson = bNewThirdPerson;
	UpdatePawnMeshes();

	if (Role < ROLE_Authority)
	{
		ServerSetThirdPerson(bNewThirdPerson);
	}
}

bool ACharacterBase::ServerSetThirdPerson_Validate(bool bNewThirdPerson)
{
	return true;
}

void ACharacterBase::ServerSetThirdPerson_Implementation(bool bNewThirdPerson)
{
	SetThirdPerson(bNewThirdPerson);
}

void ACharacterBase::OnThirdPerson()
{
	auto MyPC = Cast<AGamePlayerController>(Controller);
	if (MyPC && MyPC->IsGameInputAllowed())
	{
		SetThirdPerson(true);
	}
}

void ACharacterBase::OnThirdPersonToggle()
{
	auto MyPC = Cast<AGamePlayerController>(Controller);
	if (MyPC && MyPC->IsGameInputAllowed())
	{
		SetThirdPerson(!bIsThirdPerson);
	}
}

void ACharacterBase::OnFirstPerson()
{
	SetThirdPerson(false);
}

/// Performs ray-trace to find closest looked-at UsableActor.
AActor* ACharacterBase::GetActorInView()
{
	FVector CamLoc;
	FRotator CamRot;

	if (Controller == nullptr)
	{
		return nullptr;
	}

	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance);

	FCollisionQueryParams TraceParams(TEXT("TraceUsableActor"), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;

	/* Not tracing complex uses the rough collision instead making tiny objects easier to select. */
	TraceParams.bTraceComplex = false;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f);

	return Cast<AActor>(Hit.GetActor());
}