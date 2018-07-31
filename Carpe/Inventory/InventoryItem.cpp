#include "Inventory/InventoryItem.h"
#include "Carpe.h"
#include "Particles/ParticleSystemComponent.h"
#include "ItemInterationMessageTrigger.h"
#include "UI/CoreHudBase.h"

AInventoryItem::AInventoryItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MeshWorld = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	MeshWorld->SetSimulatePhysics(true);
	RootComponent = MeshWorld;

	/* Ignore Pawn - this is to prevent objects shooting through the level or pawns glitching on top of small items. */
	MeshWorld->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	PickupPSC = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("PickupFX"));
	PickupPSC->bAutoActivate = false;
	PickupPSC->bAutoDestroy = false;
	PickupPSC->SetupAttachment(RootComponent);

	RespawnTime = 0.0f;
	bIsActive = false;
	PickedUpBy = nullptr;

	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;

	bIsFocused = false;

	// Bound sphere setup
	bIsOverlapped = false;
	InteractiveMessage = "Press 'E' to pickup weapon";

	Sphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Sphere"));
	Sphere->bGenerateOverlapEvents = true;
	//Sphere->SetActorScale3D(FVector(3, 3, 3));
	Sphere->SetSphereRadius(50.0f);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AInventoryItem::TriggerEnter);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AInventoryItem::TriggerExit);
	Sphere->SetupAttachment(RootComponent);
	//Sphere->RegisterComponent();
}

void AInventoryItem::OnUsed(APawn* InstigatorPawn)
{
	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());

	bIsActive = false;
	OnPickedUp();

	if (RespawnTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_RespawnPickup, this, &AInventoryItem::RespawnPickup, RespawnTime, false);
	}
	else
	{
		/* Delete from level if respawn is not allowed */
		Destroy();
	}
}

void AInventoryItem::OnBeginFocus()
{
	// Used by custom PostProcess to render outlines
	if (MeshWorld)
	{
		MeshWorld->SetRenderCustomDepth(true);
	}
	bIsFocused = true;

	if (bIsOverlapped)
	{
		auto controller = GetWorld()->GetFirstPlayerController();
		auto hud = controller ? Cast<ACoreHudBase>(controller->GetHUD()) : nullptr;
		if (hud)
		{
			hud->SetMessageText(InteractiveMessage);
			hud->SetMessageVisible(true);
		}
	}
}

void AInventoryItem::OnEndFocus()
{
	// Used by custom PostProcess to render outlines
	if (MeshWorld)
	{
		MeshWorld->SetRenderCustomDepth(false);
	}
	bIsFocused = false;

	auto controller = GetWorld()->GetFirstPlayerController();
	auto hud = controller ? Cast<ACoreHudBase>(controller->GetHUD()) : nullptr;
	if (hud)
	{
		hud->SetMessageVisible(false);
	}
}

void AInventoryItem::BeginPlay()
{
	Super::BeginPlay();

	RespawnPickup();

	// register on pickup list (server only), don't care about unregistering (in FinishDestroy) - no streaming
	auto GameMode = GetWorld()->GetAuthGameMode<ACoreGameModeBase>();
	if (GameMode)
	{
		GameMode->LevelInventoryItems.Add(this);
	}
}

void AInventoryItem::NotifyActorBeginOverlap(class AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);
	if (bIsPickupOnTouch)
	{
		PickupOnTouch(Cast<ACharacterBase>(Other));
	}
}

bool AInventoryItem::CanBePickedUp(class ACharacterBase* TestPawn) const
{
	return (TestPawn && TestPawn->IsAlive()) && (bIsFocused && bIsOverlapped);
}

void AInventoryItem::GivePickupTo(class ACharacterBase* Pawn)
{
	// Implemented in child
}

void AInventoryItem::PickupOnTouch(class ACharacterBase* Pawn)
{
	if (bIsActive && Pawn && Pawn->IsAlive() && !IsPendingKill())
	{
		if (CanBePickedUp(Pawn))
		{
			GivePickupTo(Pawn);
			PickedUpBy = Pawn;

			if (!IsPendingKill())
			{
				bIsActive = false;
				OnPickedUp();

				if (RespawnTime > 0.0f)
				{
					GetWorldTimerManager().SetTimer(TimerHandle_RespawnPickup, this, &AInventoryItem::RespawnPickup, RespawnTime, false);
				}
			}
		}
	}
}

void AInventoryItem::RespawnPickup()
{
	bIsActive = true;
	PickedUpBy = nullptr;
	OnRespawned();

	TArray<AActor*> OverlappingPawns;
	GetOverlappingActors(OverlappingPawns, ACharacterBase::StaticClass());

	for (int32 i = 0; i < OverlappingPawns.Num(); i++)
	{
		PickupOnTouch(Cast<ACharacterBase>(OverlappingPawns[i]));
	}
}

void AInventoryItem::OnPickedUp()
{
	if (RespawningFX)
	{
		PickupPSC->SetTemplate(RespawningFX);
		PickupPSC->ActivateSystem();
	}
	else
	{
		PickupPSC->DeactivateSystem();
	}

	if (PickupSound && PickedUpBy)
	{
		UGameplayStatics::SpawnSoundAttached(PickupSound, PickedUpBy->GetRootComponent());
	}

	if (MeshWorld)
	{
		MeshWorld->SetVisibility(false);
		MeshWorld->SetSimulatePhysics(false);
		MeshWorld->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (Sphere)
	{
		Sphere->Deactivate();
	}

	TriggerExit(nullptr, nullptr, nullptr, 0);
	bIsFocused = false;

	OnPickedUpEvent();
}

void AInventoryItem::OnRespawned()
{
	if (ActiveFX)
	{
		PickupPSC->SetTemplate(ActiveFX);
		PickupPSC->ActivateSystem();
	}
	else
	{
		PickupPSC->DeactivateSystem();
	}

	const bool bJustSpawned = CreationTime <= (GetWorld()->GetTimeSeconds() + 5.0f);
	if (RespawnSound && !bJustSpawned)
	{
		UGameplayStatics::PlaySoundAtLocation(this, RespawnSound, GetActorLocation());
	}

	if (MeshWorld)
	{
		MeshWorld->SetVisibility(true);
		MeshWorld->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	OnRespawnEvent();
}

bool AInventoryItem::IsAttachedToPawn() const
{
	return bIsEquipped || bPendingEquip;
}

void AInventoryItem::DetachMeshFromPawn()
{
	if (MeshMain)
	{
		MeshMain->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		MeshMain->SetHiddenInGame(true);
	}
}

void AInventoryItem::SetOwningPawn(ACharacterBase* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
		// net owner for RPC calls
		SetOwner(NewOwner);
	}
}

void AInventoryItem::OnRep_IsActive()
{
	if (bIsActive)
	{
		OnRespawned();
	}
	else
	{
		OnPickedUp();
	}
}

void AInventoryItem::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInventoryItem, bIsActive);
	DOREPLIFETIME(AInventoryItem, PickedUpBy);
}

/// Bound sphere methods
void AInventoryItem::TriggerEnter(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult & SweepResult)
{
	bIsOverlapped = true;

	if (bIsFocused)
	{
		auto controller = GetWorld()->GetFirstPlayerController();
		auto hud = controller ? Cast<ACoreHudBase>(controller->GetHUD()) : nullptr;
		if (hud)
		{
			hud->SetMessageText(InteractiveMessage);
			hud->SetMessageVisible(true);
		}
	}
}

void AInventoryItem::TriggerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsOverlapped = false;
	auto controller = GetWorld()->GetFirstPlayerController();
	auto hud = controller ? Cast<ACoreHudBase>(controller->GetHUD()) : nullptr;
	if (hud)
	{
		hud->SetMessageVisible(false);
	}
}

//////////////////////////////////////////////////////////////////////////
// Replication & effects

void AInventoryItem::OnRep_MyPawn()
{
	if (MyPawn)
	{
		OnEnterInventory(MyPawn);
	}
	else
	{
		OnLeaveInventory();
	}
}

void AInventoryItem::OnEnterInventory(ACharacterBase* NewOwner)
{
	SetOwningPawn(NewOwner);
}

void AInventoryItem::OnLeaveInventory()
{
	if (Role == ROLE_Authority)
	{
		SetOwningPawn(nullptr);
	}

	if (IsAttachedToPawn())
	{
		OnUnEquip();
	}
}

void AInventoryItem::OnUnEquip()
{
	DetachMeshFromPawn();
	bIsEquipped = false;

	bPendingEquip = false;
}