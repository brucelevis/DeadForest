#include "GoalReload.h"
#include "InventoryData.hpp"
#include "HumanBase.hpp"
#include "SensoryMemory.h"
#include "GoalAvoid.h"
#include "InputReload.hpp"

using namespace realtrick::client;
using namespace realtrick;
USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalReload::GoalReload(HumanBase* owner)
	:
	GoalCompositeBase(owner)
{
	setGoalType(GoalType::RELOAD);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalReload::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	_startTime = std::chrono::system_clock::now().time_since_epoch();

	InputReload cmd(_owner);
	cmd.execute();

	addSubgoal(new GoalAvoid(_owner));
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalReload::process()
{
	if (isInactive()) activate();

	processSubgoals();
	std::chrono::duration<double> endTime = std::chrono::system_clock::now().time_since_epoch();

	// Wait for equipping item.
	if ((endTime - _startTime).count() > 1.0)
	{
		setGoalStatus(GoalStatus::COMPLETED);
		return getGoalStatus();
	}

	return getGoalStatus();
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalReload::terminate()
{
	removeAllSubgoals();
}


int GoalReload::evaluate(HumanBase* const owner)
{
	if (owner->getEquipedWeapon() == nullptr)
		return 0;

	if (owner->getEquipedWeapon()->getEntityType() == EntityType::ITEM_M1897 ||
		owner->getEquipedWeapon()->getEntityType() == EntityType::ITEM_GLOCK17 ||
		owner->getEquipedWeapon()->getEntityType() == EntityType::ITEM_M16A2)
	{
		cocos2d::log("reserved bullet : %d", owner->getEquipedWeapon()->getReservedBullets());

		// If there are some bullets already reloaded
		if (owner->getEquipedWeapon()->getReservedBullets() > 0)
		{
			return 0;
		}
		// If no bullets are reloaded
		else
		{
			EntityType bulletType = owner->getEquipedWeapon()->getBulletType();
			int amount = owner->getInventoryData()->getItemAmount(bulletType);

			cocos2d::log("bullet in inventory : %d", amount);

			// If there are some bullets remain in inventory
			if (amount > 0)
				return 100;
			else
				return 0;
		}	
	}

	return 0;
}