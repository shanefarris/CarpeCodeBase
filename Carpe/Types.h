
#pragma once
#include "Player/CharacterBase.h"
#include "Types.generated.h"

namespace EMatchState
{
	enum Type
	{
		Warmup,
		Playing,
		Won,
		Lost,
	};
}

namespace ECrosshairDirection
{
	enum Type
	{
		Left=0,
		Right=1,
		Top=2,
		Bottom=3,
		Center=4
	};
}

namespace EHudPosition
{
	enum Type
	{
		Left=0,
		FrontLeft=1,
		Front=2,
		FrontRight=3,
		Right=4,
		BackRight=5,
		Back=6,
		BackLeft=7,
	};
}



// keep in sync with ShooterImpactEffect
UENUM()
namespace EPhysMaterialType
{
	enum Type
	{
		Unknown,
		Concrete,
		Dirt,
		Water,
		Metal,
		Wood,
		Grass,
		Glass,
		Flesh,
	};
}

namespace EDialogType
{
	enum Type
	{
		None,
		Generic,
		ControllerDisconnected
	};
}

#define GAME_SURFACE_Default	SurfaceType_Default
#define GAME_SURFACE_Concrete	SurfaceType1
#define GAME_SURFACE_Dirt		SurfaceType2
#define GAME_SURFACE_Water		SurfaceType3
#define GAME_SURFACE_Metal		SurfaceType4
#define GAME_SURFACE_Wood		SurfaceType5
#define GAME_SURFACE_Grass		SurfaceType6
#define GAME_SURFACE_Glass		SurfaceType7
#define GAME_SURFACE_Body		SurfaceType8
#define GAME_SURFACE_Head		SurfaceType9
#define GAME_SURFACE_Limb		SurfaceType10

USTRUCT()
struct FDecalStruct
{
	GENERATED_USTRUCT_BODY()

	/** material */
	UPROPERTY(EditDefaultsOnly, Category= "CFG|Decal")
	UMaterial* DecalMaterial;

	/** quad size (width & height) */
	UPROPERTY(EditDefaultsOnly, Category= "CFG|Decal")
	float DecalSize;

	/** lifespan */
	UPROPERTY(EditDefaultsOnly, Category= "CFG|Decal")
	float LifeSpan;

	/** defaults */
	FDecalStruct()
		: DecalSize(256.f)
		, LifeSpan(10.f)
	{
	}
};

