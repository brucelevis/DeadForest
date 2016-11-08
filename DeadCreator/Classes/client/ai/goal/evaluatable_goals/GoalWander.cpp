#include "GoalWander.hpp"
#include "HumanBase.hpp"
#include "Game.hpp"
#include "EntityManager.hpp"
#include "EntityBase.hpp"
#include "GoalMoveToPosition.hpp"
#include "GoalEquipWeapon.hpp"
#include "SensoryMemory.hpp"
#include "InventoryData.hpp"
#include "InputCommands.hpp"
#include "AbstTargetingSystem.hpp"

using namespace realtrick::client;
using namespace realtrick;
using namespace std::chrono;
USING_NS_CC;

namespace
{
	float kDefaultDistance = 300.0f;
	float kRandomErrorRange = 100.0f;
	float kWaitingTime = 2.0f;
}


GoalWander::GoalWander(HumanBase* owner, float character_bias)
	:
	GoalEvaluatable(owner, character_bias)
{
    setGoalType(GoalType::WANDER);
}

void GoalWander::activate()
{
    setGoalStatus(GoalStatus::ACTIVE);

	removeAllSubgoals();

	Vec2 randomDirection = 
		Vec2(random(-100, 100),
			random(-100, 100)).getNormalized();

	_destination =
		_owner->getWorldPosition() 
		+ randomDirection * (kDefaultDistance + random(-kRandomErrorRange, kRandomErrorRange));

	if (_owner->getGame()->isCollideSimpleWalls(_destination))
	{
		setGoalStatus(GoalStatus::COMPLETED);
		return;
	}

	_startTime = system_clock::now().time_since_epoch();

	addSubgoal(new GoalMoveToPosition(_owner, _destination));
}


GoalStatus GoalWander::process()
{
	if (isInactive())
		activate();
    
	if (_owner->getTargetSys()->isTargetPresent())
		return GoalStatus::COMPLETED;
	
	duration<double> endTime = system_clock::now().time_since_epoch();

	// Wait for equipping item.
	if ((endTime - _startTime).count() > kWaitingTime)
	{
		setGoalStatus(processSubgoals());
	}
	
    return getGoalStatus();
}

void GoalWander::terminate()
{
	removeAllSubgoals();
}

int GoalWander::evaluate(HumanBase* const owner)
{
	if(_owner->getTargetSys()->isTargetPresent())
		return 0;
	return 100;
}


