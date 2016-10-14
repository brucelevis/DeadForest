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
GoalReload::GoalReload(HumanBase* owner, float character_bias)
	:
	GoalEvaluatable(owner, character_bias)
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
	if ((endTime - _startTime).count() > 0.6f)
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
		// If there are some bullets already reloaded
		if (owner->getEquipedWeapon()->getNumOfLeftRounds() > 0)
		{
			return 0;
		}
		// If no bullets are reloaded
		else
		{
			EntityType bulletType = owner->getEquipedWeapon()->getBulletType();
			int amount = owner->getInventoryData()->getItemAmount(bulletType);

			// If there are some bullets remain in inventory
			if (amount > 0)
				return 100;
			else
				return 0;
		}	
	}

	return 0;
}