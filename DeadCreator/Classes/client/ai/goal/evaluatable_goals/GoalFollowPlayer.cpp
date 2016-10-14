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

using namespace realtrick;
using namespace realtrick::client;

GoalFollowPlayer::GoalFollowPlayer(HumanBase* const owner, float character_bias)
	:
	GoalEvaluatable(owner, character_bias)
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

	cocos2d::Vec2 avoidMove = _owner->getSensoryMemory()->avoidingEnemiesVector(_owner);
	cocos2d::Vec2 ownerPos = _owner->getWorldPosition();

	auto player = _owner->getGame()->getPlayerPtr();

	cocos2d::Vec2 playerPos = player->getWorldPosition();
	cocos2d::Vec2 toPlayer = (playerPos - ownerPos).getNormalized() * 2;

	cocos2d::Vec2 move = (toPlayer + avoidMove).getNormalized() * 300;
	
	addSubgoal(new GoalMoveToPosition(_owner, ownerPos + move));
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

	int weight = 1;
	if (distance < 300.0f)
		weight = 1;
	else
		weight = 1 + (int)(distance / 10.0f);

	return weight;
}