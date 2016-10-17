//
//  GoalFollowPlayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalFollowPlayer.hpp"
#include "GoalAttackTarget.hpp"
#include "GoalMoveToPosition.hpp"
#include "AbstTargetingSystem.h"
#include "SensoryMemory.h"
#include "HumanBase.hpp"
#include "Game.hpp"
#include "GoalRangeAttack.h"
#include "GoalNetwork.h"
#include "GoalWalkWithAim.h"
#include "InputCommands.hpp"

using namespace realtrick;
using namespace realtrick::client;

GoalFollowPlayer::GoalFollowPlayer(HumanBase* const owner, float character_bias)
	:
	GoalEvaluatable(owner, character_bias),
	_arriveSafeZone(false)
{
	setGoalType(GoalType::FOLLOW_PLAYER);
}


//------------------------------- Activate ------------------------------------
//-----------------------------------------------------------------------------
void GoalFollowPlayer::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	//if this goal is reactivated then there may be some existing subgoals that
	//must be removed
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
GoalStatus GoalFollowPlayer::process()
{
	//if status is INACTIVE, call activate()
	if (isInactive())
		activate();

	//cocos2d::Vec2 pos = _owner->getWorldPosition();
	//float arriveRange = 150.0f;

	setGoalStatus(processSubgoals());

	return getGoalStatus();

	/*if (pos.distance(_destination) < arriveRange)
	{
		if (!_arriveSafeZone)
		{
			_arriveSafeZone = true;
			removeAllSubgoals();

			cocos2d::Vec2 aimHeading = _owner->getHeading();
			auto leader = _owner->getTargetSys()->getLeader();

			if (leader != nullptr && leader->isAlive())
				aimHeading = leader->getHeading();

			addSubgoal(new GoalWalkWithAim(_owner, _destination, aimHeading, 0.35f));
		}
	}
	
	if (_arriveSafeZone)
		return getGoalStatus();
	else
		return GoalStatus::ACTIVE;
		*/
}


void GoalFollowPlayer::terminate()
{
	removeAllSubgoals();

	InputBezelEnd bezelEnd(_owner);
	bezelEnd.execute();

	InputMoveEnd moveEnd(_owner);
	moveEnd.execute();
}

int GoalFollowPlayer::evaluate(HumanBase* const owner)
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