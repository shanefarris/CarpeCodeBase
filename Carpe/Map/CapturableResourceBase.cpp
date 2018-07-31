#include "Map/CapturableResourceBase.h"
#include "Carpe.h"

ACapturableResourceBase::ACapturableResourceBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UCapsuleComponent* CollisionComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("CollisionComp"));
	CollisionComp->InitCapsuleSize(40.0f, 50.0f);
	CollisionComp->SetCollisionObjectType(COLLISION_PICKUP2);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = CollisionComp;
}

void ACapturableResourceBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACapturableResourceBase::StartCapture()
{
}

void ACapturableResourceBase::UpdateCapture()
{
}

void ACapturableResourceBase::EndCapture()
{
}