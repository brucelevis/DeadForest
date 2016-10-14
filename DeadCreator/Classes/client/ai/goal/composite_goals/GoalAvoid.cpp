#include "GoalAvoid.h"
#include "GoalSeekToPosition.hpp"
#include "HumanBase.hpp"
#include "AbstTargetingSystem.h"
#include "SensoryMemory.h"
#include "InputMoveEnd.hpp"

using namespace realtrick::client;
using namespace realtrick;
USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalAvoid::GoalAvoid(HumanBase* owner)
	:
	GoalCompositeBase(owner)
{
	setGoalType(GoalType::AVOID);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalAvoid::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	cocos2d::Vec2 ownerPos = _owner->getWorldPosition();
	cocos2d::Vec2 avoidMove(ownerPos);

	// Avoid from current target enemy

	if (_owner->getTargetSys()->getTarget() == nullptr)
	{
		setGoalStatus(GoalStatus::COMPLETED);
		return;
	}
	
	avoidMove = (_owner->getTargetSys()->getTarget()->getWorldPosition() - ownerPos).getPerp().getNormalized() * 150;

	addSubgoal(new GoalSeekToPosition(_owner, _owner->getWorldPosition() + avoidMove));
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalAvoid::process()
{
	// If status is INACTIVE, call activate()
	if (isInactive())
		activate();

	if (getGoalStatus() == GoalStatus::COMPLETED)
		return GoalStatus::COMPLETED;

	processSubgoals();
	
	return GoalStatus::ACTIVE;
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalAvoid::terminate()
{
	InputMoveEnd moveEnd(_owner);
	moveEnd.execute();
}


