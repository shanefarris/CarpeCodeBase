#include "CoreHudBase.h"
#include "Carpe.h"
#include "Player/CharacterBase.h"
#include "CoreHudComponent.h"
#include "Inventory/Weapon_Instant.h"
#include "Player/GamePlayerController.h"

const float ACoreHudBase::MinHudScale = 0.5f;

ACoreHudBase::ACoreHudBase(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	/* You can use the FObjectFinder in C++ to reference content directly in code. Although it's advisable to avoid this and instead assign content through Blueprint child classes. */
	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDCenterDotObj(TEXT("/Game/UI/HUD/Crosshair_Basic.Crosshair_Basic"));
	CenterDotIcon = UCanvas::MakeIcon(HUDCenterDotObj.Object);

	static ConstructorHelpers::FObjectFinder<UFont> NormalFontOb(TEXT("/Game/UI/HUD/Roboto18"));
	NormalFont = NormalFontOb.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> HitTextureOb(TEXT("/Game/UI/HUD/HitIndicator"));
	HitNotifyTexture = HitTextureOb.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDMainTextureOb(TEXT("/Game/UI/HUD/HUDMain"));
	HUDMainTexture = HUDMainTextureOb.Object;

	CurrentInteractiveMessage = "Press 'E' to pick up";

	LastEnemyHitDisplayTime = 0.2f;
	LastEnemyHitTime = -LastEnemyHitDisplayTime;
	HitNotifyDisplayTime = 0.75f;

	Offsets[EHudPosition::Left] = FVector2D(173, 0);
	Offsets[EHudPosition::FrontLeft] = FVector2D(120, 125);
	Offsets[EHudPosition::Front] = FVector2D(0, 173);
	Offsets[EHudPosition::FrontRight] = FVector2D(-120, 125);
	Offsets[EHudPosition::Right] = FVector2D(-173, 0);
	Offsets[EHudPosition::BackRight] = FVector2D(-120, -125);
	Offsets[EHudPosition::Back] = FVector2D(0, -173);
	Offsets[EHudPosition::BackLeft] = FVector2D(120, -125);

	HitNotifyIcon[EHudPosition::Left] = UCanvas::MakeIcon(HitNotifyTexture, 158, 831, 585, 392);
	HitNotifyIcon[EHudPosition::FrontLeft] = UCanvas::MakeIcon(HitNotifyTexture, 369, 434, 460, 378);
	HitNotifyIcon[EHudPosition::Front] = UCanvas::MakeIcon(HitNotifyTexture, 848, 284, 361, 395);
	HitNotifyIcon[EHudPosition::FrontRight] = UCanvas::MakeIcon(HitNotifyTexture, 1212, 397, 427, 394);
	HitNotifyIcon[EHudPosition::Right] = UCanvas::MakeIcon(HitNotifyTexture, 1350, 844, 547, 321);
	HitNotifyIcon[EHudPosition::BackRight] = UCanvas::MakeIcon(HitNotifyTexture, 1232, 1241, 458, 341);
	HitNotifyIcon[EHudPosition::Back] = UCanvas::MakeIcon(HitNotifyTexture, 862, 1384, 353, 408);
	HitNotifyIcon[EHudPosition::BackLeft] = UCanvas::MakeIcon(HitNotifyTexture, 454, 1251, 371, 410);

	DefaultCrosshair[ECrosshairDirection::Left] = UCanvas::MakeIcon(HUDMainTexture, 43, 402, 25, 9); // left
	DefaultCrosshair[ECrosshairDirection::Right] = UCanvas::MakeIcon(HUDMainTexture, 88, 402, 25, 9); // right
	DefaultCrosshair[ECrosshairDirection::Top] = UCanvas::MakeIcon(HUDMainTexture, 74, 371, 9, 25); // top
	DefaultCrosshair[ECrosshairDirection::Bottom] = UCanvas::MakeIcon(HUDMainTexture, 74, 415, 9, 25); // bottom
	DefaultCrosshair[ECrosshairDirection::Center] = UCanvas::MakeIcon(HUDMainTexture, 75, 403, 7, 7); // center
}

void ACoreHudBase::DrawHUD()
{
	Super::DrawHUD();
	if (Canvas == nullptr)
	{
		return;
	}
	ScaleUI = Canvas->ClipY / 1080.0f;

	// make any adjustments for splitscreen
	int32 SSPlayerIndex = 0;
	if (PlayerOwner && PlayerOwner->IsSplitscreenPlayer(&SSPlayerIndex))
	{
		ULocalPlayer* const LP = Cast<ULocalPlayer>(PlayerOwner->Player);
		if (LP)
		{
			const ESplitScreenType::Type SSType = LP->ViewportClient->GetCurrentSplitscreenConfiguration();

			if ((SSType == ESplitScreenType::TwoPlayer_Horizontal) ||
				(SSType == ESplitScreenType::ThreePlayer_FavorBottom && SSPlayerIndex == 2) ||
				(SSType == ESplitScreenType::ThreePlayer_FavorTop && SSPlayerIndex == 0))
			{
				// full-width splitscreen viewports can handle same size HUD elements as full-screen viewports
				ScaleUI *= 2.f;
			}
		}
	}

	// Empty the info item array
	//InfoItems.Empty();
	float TextScale = 1.0f;
	// enforce min
	ScaleUI = FMath::Max(ScaleUI, MinHudScale);

	DrawCrosshair();
	DrawHitIndicator();

	for (auto Elem : CoreHudComponents)
	{
		Elem.Value->HudUpdate();
	}

	if (bPickupMessageVisible)
	{
		DrawMessage();
	}
}

void ACoreHudBase::NotifyWeaponHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator)
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	auto MyPawn = (PlayerOwner) ? Cast<ACharacterBase>(PlayerOwner->GetPawn()) : nullptr;
	if (MyPawn)
	{
		if (CurrentTime - LastHitTime > HitNotifyDisplayTime)
		{
			for (uint8 i = 0; i < 8; i++)
			{
				HitNotifyData[i].HitPercentage = 0;
			}
		}

		FVector ImpulseDir;
		FHitResult Hit;
		DamageEvent.GetBestHitInfo(this, PawnInstigator, Hit, ImpulseDir);

		//check hit vector against pre-defined direction vectors - left, front, right, back
		const FVector HitVector = FRotationMatrix(PlayerOwner->GetControlRotation()).InverseTransformVector(ImpulseDir);

		int32 DirIndex = -1;
		float HighestModifier = 0;

		for (uint8 i = 0; i < 8; i++)
		{
			//Normalize our direction vectors
			Dirs2[i].Normalize();
			const float DirModifier = FMath::Max(0.0f, FVector::DotProduct(Dirs2[i], HitVector));
			if (DirModifier > HighestModifier)
			{
				DirIndex = i;
				HighestModifier = DirModifier;
			}
		}
		if (DirIndex > -1)
		{
			const float DamageTakenPercentage = (DamageTaken / MyPawn->Health);
			HitNotifyData[DirIndex].HitPercentage += DamageTakenPercentage * 2;
			HitNotifyData[DirIndex].HitPercentage = FMath::Clamp(HitNotifyData[DirIndex].HitPercentage, 0.0f, 1.0f);
			HitNotifyData[DirIndex].HitTime = CurrentTime;
		}

	}

	LastHitTime = CurrentTime;
}

void ACoreHudBase::NotifyEnemyHit()
{
	LastEnemyHitTime = GetWorld()->GetTimeSeconds();
}

void ACoreHudBase::DrawHitIndicator()
{
	if (HitNotifyTexture == nullptr)
	{
		return;
	}

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastHitTime <= HitNotifyDisplayTime)
	{
		const float StartX = Canvas->ClipX / 2.0f;
		const float StartY = Canvas->ClipY / 2.0f;

		for (uint8 i = 0; i < 8; i++)
		{
			const float TimeModifier = FMath::Max(0.0f, 1 - (CurrentTime - HitNotifyData[i].HitTime) / HitNotifyDisplayTime);
			const float Alpha = TimeModifier * HitNotifyData[i].HitPercentage;
			Canvas->SetDrawColor(255, 255, 255, FMath::Clamp(FMath::TruncToInt(Alpha * 255 * 1.5f), 0, 255));
			Canvas->DrawIcon(HitNotifyIcon[i],
				StartX + (HitNotifyIcon[i].U - HitNotifyTexture->GetSizeX() / 2 + Offsets[i].X) * ScaleUI,
				StartY + (HitNotifyIcon[i].V - HitNotifyTexture->GetSizeY() / 2 + Offsets[i].Y) * ScaleUI,
				ScaleUI);
		}
	}
}

void ACoreHudBase::SetMessageVisible(bool bVisible)
{
	bPickupMessageVisible = bVisible;
}

void ACoreHudBase::SetMessageText(const FString& Message)
{
	CurrentInteractiveMessage = Message;
}

void ACoreHudBase::DrawMessage()
{
	float CanvasCentre = Canvas->ClipX / 2.0f;
	float X = 0.0f;
	float TextScale = 1.0f;
	float SizeX, SizeY;

	FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), NormalFont, FColor::White);
	TextItem.EnableShadow(FLinearColor::Black);
	TextItem.Text = FText::FromString(CurrentInteractiveMessage);
	TextItem.Scale = FVector2D(TextScale * ScaleUI, TextScale * ScaleUI);
	TextItem.FontRenderInfo = ShadowedFont;

	Canvas->StrLen(TextItem.Font, TextItem.Text.ToString(), SizeX, SizeY);
	X = CanvasCentre - (SizeX * TextItem.Scale.X) / 2.0f;
	Canvas->DrawItem(TextItem, X, Canvas->ClipY / 1.5f);
}

void ACoreHudBase::DrawCrosshair()
{
#if 1
	auto PCOwner = Cast<AGamePlayerController>(PlayerOwner);
	if (PCOwner)
	{
		auto Pawn = Cast<ACharacterBase>(PCOwner->GetPawn());
		if (Pawn && Pawn->GetWeapon() && !Pawn->IsRunning()
			&& (Pawn->IsTargeting() || (!Pawn->IsTargeting() && !Pawn->GetWeapon()->bHideCrosshairWhileNotAiming)))
		{
			const float SpreadMulti = 300;
			auto InstantWeapon = Cast<AWeapon_Instant>(Pawn->GetWeapon());
			auto MyWeapon = Pawn->GetWeapon();
			const float CurrentTime = GetWorld()->GetTimeSeconds();

			float AnimOffset = 0;
			if (MyWeapon)
			{
				const float EquipStartedTime = MyWeapon->GetEquipStartedTime();
				const float EquipDuration = MyWeapon->GetEquipDuration();
				AnimOffset = 300 * (1.0f - FMath::Min(1.0f, (CurrentTime - EquipStartedTime) / EquipDuration));
			}
			float CrossSpread = 2 + AnimOffset;
			if (InstantWeapon != nullptr)
			{
				CrossSpread += SpreadMulti*FMath::Tan(FMath::DegreesToRadians(InstantWeapon->GetCurrentSpread()));
			}
			float CenterX = Canvas->ClipX / 2;
			float CenterY = Canvas->ClipY / 2;
			Canvas->SetDrawColor(255, 255, 255, 192);

			FCanvasIcon* CurrentCrosshair[5];
			for (int32 i = 0; i< 5; i++)
			{
				if (MyWeapon && MyWeapon->UseCustomAimingCrosshair && Pawn->IsTargeting())
				{
					CurrentCrosshair[i] = &MyWeapon->AimingCrosshair[i];
				}
				else if (MyWeapon && MyWeapon->UseCustomCrosshair)
				{
					CurrentCrosshair[i] = &MyWeapon->Crosshair[i];
				}
				else
				{
					CurrentCrosshair[i] = &DefaultCrosshair[i];
				}

				if (CurrentCrosshair[i] == nullptr)
				{
					return;
				}
			}

			if (Pawn->IsTargeting() && MyWeapon->UseLaserDot)
			{
				Canvas->SetDrawColor(255, 0, 0, 192);
				Canvas->DrawIcon(*CurrentCrosshair[ECrosshairDirection::Center],
					CenterX - (*CurrentCrosshair[ECrosshairDirection::Center]).UL*ScaleUI / 2.0f,
					CenterY - (*CurrentCrosshair[ECrosshairDirection::Center]).VL*ScaleUI / 2.0f, ScaleUI);
			}
			else
			{
				Canvas->DrawIcon(*CurrentCrosshair[ECrosshairDirection::Center],
					CenterX - (*CurrentCrosshair[ECrosshairDirection::Center]).UL*ScaleUI / 2.0f,
					CenterY - (*CurrentCrosshair[ECrosshairDirection::Center]).VL*ScaleUI / 2.0f, ScaleUI);

				Canvas->DrawIcon(*CurrentCrosshair[ECrosshairDirection::Left],
					CenterX - 1 - (*CurrentCrosshair[ECrosshairDirection::Left]).UL * ScaleUI - CrossSpread * ScaleUI,
					CenterY - (*CurrentCrosshair[ECrosshairDirection::Left]).VL*ScaleUI / 2.0f, ScaleUI);
				Canvas->DrawIcon(*CurrentCrosshair[ECrosshairDirection::Right],
					CenterX + CrossSpread * ScaleUI,
					CenterY - (*CurrentCrosshair[ECrosshairDirection::Right]).VL * ScaleUI / 2.0f, ScaleUI);

				Canvas->DrawIcon(*CurrentCrosshair[ECrosshairDirection::Top],
					CenterX - (*CurrentCrosshair[ECrosshairDirection::Top]).UL * ScaleUI / 2.0f,
					CenterY - 1 - (*CurrentCrosshair[ECrosshairDirection::Top]).VL * ScaleUI - CrossSpread * ScaleUI, ScaleUI);
				Canvas->DrawIcon(*CurrentCrosshair[ECrosshairDirection::Bottom],
					CenterX - (*CurrentCrosshair[ECrosshairDirection::Bottom]).UL * ScaleUI / 2.0f,
					CenterY + CrossSpread * ScaleUI, ScaleUI);
			}

			if (CurrentTime - LastEnemyHitTime >= 0 && CurrentTime - LastEnemyHitTime <= LastEnemyHitDisplayTime)
			{
				const float Alpha = FMath::Min(1.0f, 1 - (CurrentTime - LastEnemyHitTime) / LastEnemyHitDisplayTime);
				Canvas->SetDrawColor(255, 255, 255, 255 * Alpha);

				//Canvas->DrawIcon(HitNotifyCrosshair,
				//	CenterX - HitNotifyCrosshair.UL*ScaleUI / 2.0f,
				//	CenterY - HitNotifyCrosshair.VL*ScaleUI / 2.0f, ScaleUI);
			}
		}
	}

#else

	float CenterX = Canvas->ClipX / 2;
	float CenterY = Canvas->ClipY / 2;
	float CenterDotScale = 0.5f;

	ACharacterBase* Pawn = Cast<ACharacterBase>(GetOwningPawn());
	if (Pawn && Pawn->IsAlive())
	{
		// Boost size when hovering over a usable object.
		AActor* Usable = Pawn->GetActorInView();
		if (Usable)
		{
			CenterDotScale *= 1.5f;
		}

		Canvas->SetDrawColor(255, 255, 255, 255);
		Canvas->DrawIcon(CenterDotIcon,
			CenterX - CenterDotIcon.UL * CenterDotScale / 2.0f,
			CenterY - CenterDotIcon.VL * CenterDotScale / 2.0f, CenterDotScale);
	}
#endif
}

void ACoreHudBase::OnStateChanged_Implementation(EHUDState NewState)
{
	CurrentState = NewState;
}

EHUDState ACoreHudBase::GetCurrentState()
{
	return CurrentState;
}

void ACoreHudBase::AddHudComponent(FString Name, UCoreHudComponent* CoreHudComponent)
{
	CoreHudComponent->AddToViewport();
	CoreHudComponent->SetVisibility(ESlateVisibility::Visible);
	CoreHudComponents.Add(Name, CoreHudComponent);
}

void ACoreHudBase::RemoveHudComponent(FString Name)
{
	auto componnet = CoreHudComponents[Name];
	if (componnet)
	{
		componnet->RemoveFromViewport();
		CoreHudComponents.Remove(Name);
	}
	
}

UCoreHudComponent* ACoreHudBase::GetHudComponent(FString Name)
{
	return CoreHudComponents.FindRef(Name);
}

TArray<UCoreHudComponent*> ACoreHudBase::GetAllHudComponents()
{
	TArray<UCoreHudComponent*> Components;
	for (auto component : CoreHudComponents)
	{
		Components.Add(component.Value);
	}

	return Components;
}