#include "GoalAvoid.h"
#include "GoalSeekToPosition.hpp"
#include "HumanBase.hpp"
#include "AbstTargetingSystem.h"
#include "SensoryMemory.h"

using namespace realtrick::client;
using namespace realtrick;
USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalAvoid::GoalAvoid(HumanBase* owner)
	:
	GoalCompositeBase(owner)
{
	setGoalType(GoalType::GOAL_AVOID);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalAvoid::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	cocos2d::Vec2 avoidMove;
	cocos2d::Vec2 ownerPos = _owner->getWorldPosition();

	// Avoid from current target enemy
	avoidMove = (_owner->getTargetSys()->getTarget()->getWorldPosition() - ownerPos).getPerp().getNormalized() * 150;

	// Avoid from all enemies
	/*
	auto targetList = _owner->getSensoryMemory()->getListOfRecentlySensedOpponents();
	for (auto e : targetList)
	{
		avoidMove += (e->getWorldPosition() - ownerPos).getNormalized();
	}
	avoidMove = -avoidMove.getNormalized() * 150;
	*/

	addSubgoal(new GoalSeekToPosition(_owner, _owner->getWorldPosition() + avoidMove));
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalAvoid::process()
{
	// If status is INACTIVE, call activate()
	if (isInactive())
		activate();

	processSubgoals();
	
	return GoalStatus::ACTIVE;
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalAvoid::terminate()
{}


