#include "AI/Bot.h"
#include "Carpe.h"
#include "AI/AIControllerBase.h"

#include "Perception/PawnSensingComponent.h"

ABot::ABot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AIControllerClass = AAIControllerBase::StaticClass();

	UpdatePawnMeshes();

	bUseControllerRotationYaw = true;

	// Noise emitter for both players and enemies. This keeps track of MakeNoise data and is used by the pawnsensing component in our SZombieCharacter class
	NoiseEmitterComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitterComp"));

	// Our sensing component to detect players by visibility and noise checks.
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->HearingThreshold = 600;
	PawnSensingComp->LOSHearingThreshold = 1200;

	MeleeCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeCollision"));
	MeleeCollisionComp->SetRelativeLocation(FVector(45, 0, 25));
	MeleeCollisionComp->SetCapsuleHalfHeight(60);
	MeleeCollisionComp->SetCapsuleRadius(35, false);
	MeleeCollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeCollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MeleeCollisionComp->SetupAttachment(GetCapsuleComponent());

	Health = 100;
	MeleeDamage = 25.1f;
	MeleeStrikeCooldown = 1.0f;
	LastMeleeAttackTime = 0.0f;
	CurrentMeleeTarget = nullptr;
	bIsPunching = false;
}

bool ABot::IsFirstPerson() const
{
	return false;
}

void ABot::FaceRotation(FRotator NewRotation, float DeltaTime)
{
	FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), NewRotation, DeltaTime, 8.0f);

	Super::FaceRotation(CurrentRotation, DeltaTime);
}

void ABot::BeginPlay()
{
	Super::BeginPlay();

	/* This is the earliest moment we can bind our delegates to the component */
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &ABot::OnSeePlayer);
		PawnSensingComp->OnHearNoise.AddDynamic(this, &ABot::OnHearNoise);
	}

	if (MeleeCollisionComp)
	{
		MeleeCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABot::OnMeleeCompBeginOverlap);
	}
}

void ABot::OnMeleeCompBeginOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult & SweepResult)
{
	if (bIsDying || bPlayDeathAnim)
	{
		return;
	}
	/* Stop any running attack timers */
	TimerHandle_MeleeAttack.Invalidate();

	CurrentMeleeTarget = Cast<ACharacter>(OtherActor);
	bIsPunching = true;

	/* Set re-trigger timer to re-check overlapping pawns at melee attack rate interval */
	GetWorldTimerManager().SetTimer(TimerHandle_MeleeAttack, this, &ABot::OnRetriggerMeleeStrike, MeleeStrikeCooldown, true);
}

void ABot::OnRetriggerMeleeStrike()
{
	/* Apply damage to a single random pawn in range. */
	TArray<AActor*> Overlaps;
	bIsPunching = false;
	MeleeCollisionComp->GetOverlappingActors(Overlaps, ACharacterBase::StaticClass());
	for (int32 i = 0; i < Overlaps.Num(); i++)
	{
		ACharacterBase* OverlappingPawn = Cast<ACharacterBase>(Overlaps[i]);
		auto MyPlayerState = Cast<AGamePlayerState>(PlayerState);
		auto OverlapPlayerState = Cast<AGamePlayerState>(OverlappingPawn->PlayerState);
		if (MyPlayerState && OverlapPlayerState)
		{
			if (OverlappingPawn && OverlappingPawn != this &&
				(MyPlayerState->GetTeamNum() != OverlapPlayerState->GetTeamNum()))
			{
				CurrentMeleeTarget = OverlappingPawn;
				bIsPunching = true;
				//break; /* Uncomment to only attack one pawn maximum */
			}
		}
	}

	/* No pawns in range, cancel the retrigger timer */
	if (bIsPunching == false)
	{
		TimerHandle_MeleeAttack.Invalidate();
	}
}

void ABot::PerformMeleeStrike(AActor* HitActor)
{
	if (LastMeleeAttackTime > GetWorld()->GetTimeSeconds() - MeleeStrikeCooldown)
	{
		/* Set timer to start attacking as soon as the cooldown elapses. */
		if (!TimerHandle_MeleeAttack.IsValid())
		{
			// TODO: Set Timer
		}

		/* Attacked before cooldown expired */
		return;
	}

	// Convert the actor to a character base, and use CurrentMeleeTarget first if we have it.
	ACharacter* OtherPawn = nullptr;
	auto IsOverlapped = false;
	if (HitActor && HitActor != this && IsAlive())
	{
		
		OtherPawn = Cast<ACharacter>(HitActor);
		IsOverlapped = MeleeCollisionComp->IsOverlappingActor(HitActor);
	}
	else if (CurrentMeleeTarget)
	{
		OtherPawn = Cast<ACharacter>(CurrentMeleeTarget);
		IsOverlapped = MeleeCollisionComp->IsOverlappingActor(OtherPawn);
	}

	// Check if we are still overlapped.
	if (IsOverlapped == false)
	{
		return;
	}

	if (OtherPawn)
	{
		
		auto MyPS = Cast<AGamePlayerState>(PlayerState);
		auto OtherPS = Cast<AGamePlayerState>(OtherPawn->PlayerState);

		if (MyPS && OtherPS)
		{
			if (MyPS->GetTeamNum() == OtherPS->GetTeamNum())
			{
				/* Do not attack other zombies. */
				return;
			}

			/* Set to prevent a zombie to attack multiple times in a very short time */
			LastMeleeAttackTime = GetWorld()->GetTimeSeconds();

			FPointDamageEvent DmgEvent;
			DmgEvent.DamageTypeClass = PunchDamageType;
			DmgEvent.Damage = MeleeDamage;

			auto AIController = Cast<AAIControllerBase>(AIControllerClass);
			if (AIController != nullptr)
			{
				DmgEvent.ShotDirection = AIController->GetControlRotation().Vector();
				DmgEvent.ShotDirection.Normalize();
			}

			OtherPawn->TakeDamage(DmgEvent.Damage, DmgEvent, GetController(), this);
		}
	}
}

void ABot::PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	Super::PlayHit(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
}

void ABot::OnSeePlayer(APawn* Pawn)
{
	if (!IsAlive())
	{
		return;
	}

	/* Keep track of the time the player was last sensed in order to clear the target */
	LastSeenTime = GetWorld()->GetTimeSeconds();
	bSensedTarget = true;

	AAIControllerBase* AIController = Cast<AAIControllerBase>(GetController());
	auto SensedPawn = Cast<ACharacterBase>(Pawn);
	if (AIController && SensedPawn->IsAlive())
	{
		AIController->SetEnemy(SensedPawn);
	}
}


void ABot::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	if (!IsAlive())
	{
		return;
	}

	if (!bSensedTarget)
	{
		//BroadcastUpdateAudioLoop(true);
	}

	bSensedTarget = true;
	LastHeardTime = GetWorld()->GetTimeSeconds();

	AAIControllerBase* AIController = Cast<AAIControllerBase>(GetController());
	if (AIController)
	{
		AIController->SetEnemy(PawnInstigator);
	}
}
