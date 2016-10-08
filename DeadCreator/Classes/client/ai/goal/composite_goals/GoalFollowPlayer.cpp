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
#include "HumanBase.hpp"
#include "Game.hpp"

using namespace realtrick;
using namespace realtrick::client;

GoalFollowPlayer::GoalFollowPlayer(HumanBase* const owner)
	:
	GoalCompositeBase(owner)
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

	auto player = _owner->getGame()->getPlayerPtr();

	addSubgoal(new GoalMoveToPosition(_owner, player->getWorldPosition()));
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalFollowPlayer::process()
{
	//if status is INACTIVE, call activate()
	if (isInactive())
		activate();
	auto player = _owner->getGame()->getPlayerPtr();
	cocos2d::Vec2 pos = _owner->getWorldPosition();
	cocos2d::Vec2 destination = player->getWorldPosition();
	float arriveRange = 200.0f - cocos2d::random(0.0f, 100.0f);

	_goalStatus = processSubgoals();

	if(pos.distance(destination) < arriveRange)
		setGoalStatus(GoalStatus::COMPLETED);

	return _goalStatus;
}


void GoalFollowPlayer::terminate()
{
	removeAllSubgoals();
}

int GoalFollowPlayer::evaluate(HumanBase* const owner)
{
	auto player = _owner->getGame()->getPlayerPtr();
	float distance = owner->getWorldPosition().distance(player->getWorldPosition());

	int weight = 0;
	if (distance < 300.0f)
		weight = 0;
	else
		weight = (int)(distance / 10.0f);

	return weight;
}