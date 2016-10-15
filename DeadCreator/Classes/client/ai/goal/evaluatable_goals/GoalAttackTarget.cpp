//
//  GoalTraverseEdge.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalAttackTarget.hpp"
#include "GoalMoveToPosition.hpp"
#include "GraphEdgeTypes.h"
#include "InputCommands.hpp"
#include "PathEdge.h"
#include "HumanBase.hpp"
#include "PathPlanner.h"
#include "AbstTargetingSystem.h"
#include "GoalMainAttack.h"
#include "GoalRangeAttack.h"
#include "GoalHuntTarget.hpp"
#include "SensoryMemory.h"
#include "InventoryData.hpp"

namespace
{
	const int kStandardNumForCrowd = 6;
	const int kNoiseForCrowd = 2;
	const int kWeightForAttack = 2;
}

using namespace realtrick;
using namespace realtrick::client;


GoalAttackTarget::GoalAttackTarget(HumanBase* const owner, float character_bias)
	:
	GoalEvaluatable(owner, character_bias)
{
	setGoalType(GoalType::ATTACK_TARGET);
}


//------------------------------- Activate ------------------------------------
//-----------------------------------------------------------------------------
void GoalAttackTarget::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	//if this goal is reactivated then there may be some existing subgoals that
	//must be removed
	removeAllSubgoals();

	//it is possible for a bot's target to die whilst this goal is active so we
	//must test to make sure the bot always has an active target
	if (!_owner->getTargetSys()->isTargetPresent())
	{
		setGoalStatus(GoalStatus::COMPLETED);
		return;
	}

	//if the bot is able to shoot the target (there is LOS between bot and target),
	//then select a tactic to follow while shooting
	if (_owner->getTargetSys()->isTargetAttackable())
	{
		if(_owner->getEquipedWeapon() == nullptr) 
			addSubgoal(
				new GoalMainAttack(
					_owner,
					_owner->getTargetSys()->getTarget()->getWorldPosition(),
					GoalMainAttack::roughMoving));

		else if(_owner->getEquipedWeapon()->getEntityType() == EntityType::ITEM_AXE)
			addSubgoal(
				new GoalMainAttack(
					_owner, 
					_owner->getTargetSys()->getTarget()->getWorldPosition(),
					GoalMainAttack::smartMoving));
		else
		{
			if(_owner->getSensoryMemory()->getListOfRecentlySensedEntities(false).size() > 
				kStandardNumForCrowd + cocos2d::random(-kNoiseForCrowd, kNoiseForCrowd))
				addSubgoal(
					new GoalRangeAttack(
						_owner,
						_owner->getTargetSys()->getTarget()->getWorldPosition(),
						GoalRangeAttack::dealCrowdMoving));
			else
				addSubgoal(
					new GoalRangeAttack(
						_owner,
						_owner->getTargetSys()->getTarget()->getWorldPosition(),
						GoalRangeAttack::makeFormationMoving));
		}
			
	}

	//if the target is not attackable, go hunt it.
	else
	{
		addSubgoal(new GoalHuntTarget(_owner));
	}
}




//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalAttackTarget::process()
{
	//if status is INACTIVE, call activate()
	if (isInactive())
		activate();

	//process the subgoals
	_goalStatus = processSubgoals();

	//if any of the subgoals have FAILED then this goal re-plans
	if (_goalStatus == GoalStatus::FAILED)
	{
		setGoalStatus(GoalStatus::INACTIVE);
	}

	return _goalStatus;
}



int GoalAttackTarget::evaluate(HumanBase* const owner) 
{
	if (_owner->getTargetSys()->isTargetPresent())
		return kWeightForAttack * _character_bias;
	else
		return 0;
}
