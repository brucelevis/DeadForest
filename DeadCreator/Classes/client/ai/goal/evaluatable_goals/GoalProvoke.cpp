#include "GoalProvoke.hpp"
#include "GoalSeekToPosition.hpp"
#include "GoalMoveToPosition.hpp"
#include "GoalShake.hpp"
#include "HumanBase.hpp"
#include "AbstTargetingSystem.hpp"
#include "SensoryMemory.hpp"
#include "InventoryData.hpp"
#include "GoalHide.hpp"

using namespace realtrick::client;
using namespace realtrick;
USING_NS_CC;

namespace
{
	const float kInvadeDistance = 30.0f;
	const float kShakeTime = 1.0f;
}

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalProvoke::GoalProvoke(HumanBase* owner, float character_bias)
	:
	GoalEvaluatable(owner, character_bias)
{
    setGoalName("provoke");
	setGoalType(GoalType::PROVOKE);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalProvoke::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);
	removeAllSubgoals();

	if (!_owner->getTargetSys()->isTargetPresent())
		return;

	_savePos = _owner->getWorldPosition();
	_arrivedTargetRange = false;
	_finishedAvoid = false;

	addSubgoal(new GoalMoveToPosition(
		_owner,
		_owner->getTargetSys()->getTarget()->getWorldPosition(),
		std::make_shared<ArrivingData>(
			ArrivingData(_owner->getMoving(), 50.0f, 150.0f))));
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalProvoke::process()
{
	if (isInactive())
		activate();

	if (!_owner->getTargetSys()->isTargetPresent())
		return GoalStatus::COMPLETED;

	_goalStatus = processSubgoals();

	auto target = _owner->getTargetSys()->getTarget();

	if (!_arrivedTargetRange && 
		target->getSensoryMemory()->getViewRange() - kInvadeDistance >
		_owner->getWorldPosition().distance(target->getWorldPosition()))
	{
		_arrivedTargetRange = true;
		setGoalStatus(GoalStatus::ACTIVE);
		removeAllSubgoals();
		addSubgoal(
			new GoalShake(
				_owner,
				-(target->getWorldPosition() - _owner->getWorldPosition()).getNormalized(),
				(target->getWorldPosition() - _owner->getWorldPosition()).getNormalized(), kShakeTime));
	}

	if (_arrivedTargetRange && !_finishedAvoid && _goalStatus == GoalStatus::COMPLETED)
	{
		_finishedAvoid = true;
		setGoalStatus(GoalStatus::ACTIVE);
		removeAllSubgoals();
		addSubgoal(new GoalHide(_owner));
	}

	if (_arrivedTargetRange && _finishedAvoid)
		return _goalStatus;
	else
		return GoalStatus::ACTIVE;

}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalProvoke::terminate()
{
	removeAllSubgoals();
}


int GoalProvoke::evaluate(HumanBase* const owner)
{
	if (!owner->getTargetSys()->isTargetPresent())
		return 0;

	auto target = owner->getTargetSys()->getTarget();

	if (target->getSensoryMemory()->isEntityViewable(owner))
		return 0;

	int enemyCnt = (int)_owner->getSensoryMemory()->getListOfRecentlySensedEntities(false).size();
	int allyCnt = (int)_owner->getSensoryMemory()->getListOfRecentlySensedEntities(true).size();

	return std::max(enemyCnt - (allyCnt + 1), 0) * 10;
}
