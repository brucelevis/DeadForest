#include "GoalEquipWeapon.h"
#include "InventoryData.hpp"
#include "HumanBase.hpp"
#include "SensoryMemory.h"

using namespace realtrick::client;
using namespace realtrick;
USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalEquipWeapon::GoalEquipWeapon(HumanBase* owner, EntityType weaponType)
	:
	GoalBase(owner),
	_weaponType(weaponType)
{
	setGoalType(GoalType::GOAL_MAIN_ATTACK);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalEquipWeapon::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	_startTime = std::chrono::system_clock::now().time_since_epoch();

	_owner->useItem(_weaponType);

	if (_owner->getEquipedWeapon() != nullptr)
		_owner->getSensoryMemory()->setAttackRange(_owner->getEquipedWeapon()->getRange() - 10);
	else
		_owner->getSensoryMemory()->setAttackRange(50);
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalEquipWeapon::process()
{
	// If status is INACTIVE, call activate()
	if (isInactive())
		activate();

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
void GoalEquipWeapon::terminate()
{
	setGoalStatus(GoalStatus::COMPLETED);
}


