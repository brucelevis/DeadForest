#include "GoalRunAway.h"
#include "GoalSeekToPosition.hpp"
#include "HumanBase.hpp"
#include "AbstTargetingSystem.h"
#include "SensoryMemory.h"
#include "InventoryData.hpp"

using namespace realtrick::client;
using namespace realtrick;
USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalRunAway::GoalRunAway(HumanBase* owner, float character_bias)
	:
	GoalEvaluatable(owner, character_bias)
{
	setGoalType(GoalType::RUN_AWAY);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalRunAway::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	cocos2d::Vec2 ownerPos = _owner->getWorldPosition();
	cocos2d::Vec2 avoidMove(ownerPos);

	if (_owner->getTargetSys()->getTarget() == nullptr)
	{
		setGoalStatus(GoalStatus::COMPLETED);
		return;
	}
	
	avoidMove = (_owner->getTargetSys()->getTarget()->getWorldPosition() - ownerPos).getPerp().getNormalized() * 800;
	addSubgoal(new GoalSeekToPosition(_owner, _owner->getWorldPosition() + avoidMove));
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalRunAway::process()
{
	// If status is INACTIVE, call activate()
	if (isInactive())
		activate();

	setGoalStatus(processSubgoals());
	
	return getGoalStatus();
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalRunAway::terminate()
{
	removeAllSubgoals();
}


int GoalRunAway::evaluate(HumanBase* const owner)
{
	if (!_owner->getSensoryMemory()->isReadyToFight() && 
		_owner->getSensoryMemory()->queryUsableRangeWeapon().size() == 0 &&
		_owner->getSensoryMemory()->queryMeleeWeapon().size() == 0)
		return 3;

	return 0;
}