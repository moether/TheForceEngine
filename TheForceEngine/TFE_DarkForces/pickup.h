#pragma once
//////////////////////////////////////////////////////////////////////
// Dark Forces Pickup - 
// Items that can be picked up such as ammo, keys, extra lives,
// and weapons.
//////////////////////////////////////////////////////////////////////

#include <TFE_System/types.h>
#include <TFE_Asset/dfKeywords.h>
#include <TFE_Level/rsector.h>
#include <TFE_Level/robject.h>
#include "logic.h"
#include "itemId.h"

namespace TFE_DarkForces
{
	struct Task;

	// Note that the Pickup struct mirrors the first 24 bytes of Logic {}
	struct Pickup
	{
		Logic logic;

		ItemId id;
		s32 index;
		ItemType type;
		s32* item;
		s32* value;
		s32 amount;
		s32 msgId[2];
		s32 maxAmount;
	};

	ItemId getPickupItemId(const char* keyword);
	Logic* obj_createPickup(SecObject* obj, ItemId id);
}  // namespace TFE_DarkForces