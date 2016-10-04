//
//  GoalAttackToDestination.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalAttackToDestination.hpp"
#include "GoalAttackTarget.hpp"
#include "GoalMoveToPosition.hpp"
#include "AbstTargetingSystem.h"
#include "HumanBase.hpp"

using namespace realtrick;
using namespace realtrick::client;

GoalAttackToDestination::GoalAttackToDestination(
	HumanBase* const owner,
	const cocos2d::Vec2& destination,
	float arriveRange)
	:
	GoalCompositeBase(owner),
	_destination(destination),
	_arriveRange(arriveRange)
{
	setGoalType(GoalType::ATTACK_TO_DESTINATION);
}


//------------------------------- Activate ------------------------------------
//-----------------------------------------------------------------------------
void GoalAttackToDestination::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	//if this goal is reactivated then there may be some existing subgoals that
	//must be removed
	removeAllSubgoals();

	addSubgoal(new GoalMoveToPosition(_owner, _destination));
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalAttackToDestination::process()
{
	//if status is INACTIVE, call activate()
	if (isInactive())
		activate();

	if (_destination.x - _arriveRange < _owner->getWorldPosition().x &&
		_destination.x + _arriveRange > _owner->getWorldPosition().x &&
		_destination.y - _arriveRange < _owner->getWorldPosition().y &&
		_destination.y + _arriveRange > _owner->getWorldPosition().y)
	{
		setGoalStatus(GoalStatus::COMPLETED);
	}

	if (_owner->getTargetSys()->isTargetPresent() &&
		_subGoals.back()->getGoalType() == GoalType::MOVE_TO_POS)
	{
		addSubgoal(new GoalAttackTarget(_owner));
	}

	_goalStatus = processSubgoals();

	return _goalStatus;
}


void GoalAttackToDestination::terminate()
{
	removeAllSubgoals();
}

