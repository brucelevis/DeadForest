//
//  GoalFollowLeader.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 8. 17..
//
//

#include "Goals.hpp"
#include "AbstTargetingSystem.hpp"
#include "SensoryMemory.hpp"
#include "HumanBase.hpp"
#include "Game.hpp"
#include "InputCommands.hpp"

using namespace realtrick;
using namespace realtrick::client;

GoalFollowLeader::GoalFollowLeader(HumanBase* const owner, float character_bias)
	:
	GoalEvaluatable(owner, character_bias),
	_arriveSafeZone(false)
{
	setGoalType(GoalType::FOLLOW_PLAYER);
}


//------------------------------- Activate ------------------------------------
//-----------------------------------------------------------------------------
void GoalFollowLeader::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	// if this goal is reactivated then there may be some existing subgoals that
	// must be removed
	removeAllSubgoals();

	auto leader = _owner->getTargetSys()->getLeader();
	if (leader == nullptr)
	{
		setGoalStatus(GoalStatus::COMPLETED);
		return;
	}

	int idx = leader->getTargetSys()->queryFollowerIndex(_owner);
	if (idx < 0)
	{
		setGoalStatus(GoalStatus::COMPLETED);
		return;
	}
		
	if(!leader->isAlive())
	{
		setGoalStatus(GoalStatus::COMPLETED);
		return;
	}

	_destination = GoalNetwork::queryFormationPos(_owner, leader, idx);

	if (_owner->getWorldPosition().distance(_destination) <
		_owner->getBoundingRadius())
	{
		setGoalStatus(GoalStatus::COMPLETED);
		return;
	}

	_arriveSafeZone = false;
	addSubgoal(
		new GoalMoveToPosition(
			_owner,
			_destination,
			std::make_shared<ArrivingData>(
				ArrivingData(leader->getHeading(), 100.0f, 50.0f))));
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalFollowLeader::process()
{
	//if status is INACTIVE, call activate()
	if (isInactive())
		activate();

	setGoalStatus(processSubgoals());

	return getGoalStatus();
}


void GoalFollowLeader::terminate()
{
	removeAllSubgoals();

	InputBezelEnd bezelEnd(_owner);
	bezelEnd.execute();

	InputMoveEnd moveEnd(_owner);
	moveEnd.execute();
}

int GoalFollowLeader::evaluate(HumanBase* const owner)
{
	auto leader = _owner->getTargetSys()->getLeader();
	if (leader == nullptr)
		return 0;

	int idx = leader->getTargetSys()->queryFollowerIndex(_owner);
	if (idx < 0)
		return 0;

	float distance = owner->getWorldPosition().distance(leader->getWorldPosition());

	int weight = 1;
	if (distance < 500.0f)
		weight = 1;
	else
		weight = 1 + (int)(distance / 50.0f);

	return weight;
}
