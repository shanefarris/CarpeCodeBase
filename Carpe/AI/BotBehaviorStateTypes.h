
#pragma once
#include "BotBehaviorStateTypes.generated.h"

UENUM()
enum class EBotBehaviorStateTypes : uint8
{
	/* Does not move, remains in place until a player is spotted */
	Passive,

	/* Patrols a region until a player is spotted */
	Patrolling,
};