#include "GoalAvoidCollision.h"
#include "GoalSeekToPosition.hpp"
#include "HumanBase.hpp"
#include "AbstTargetingSystem.h"
#include "SensoryMemory.h"
#include "InputMoveBegin.hpp"
#include "InputMoveEnd.hpp"
#include "Game.hpp"
#include "SensoryMemory.h"

using namespace realtrick::client;
using namespace realtrick;
USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalAvoidCollision::GoalAvoidCollision(HumanBase* owner)
	:
	GoalCompositeBase(owner)
{
	setGoalType(GoalType::AVOID_COLLISION);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalAvoidCollision::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	// Avoid from any close entity.

	float maintainDistance = 220.0f;
	Vec2 pos = _owner->getWorldPosition();
	Vec2 avoidMove;
	auto entities = _owner->getGame()->getNeighborsOnMove(pos, maintainDistance);
	_startTime = std::chrono::system_clock::now().time_since_epoch();

	int weight = 0;
	for (auto e : entities)
	{
		if (e->getTag() == _owner->getTag())
			continue;

		if (!isMasked(e->getFamilyMask(), FamilyMask::HUMAN_BASE))
			continue;

		if (_owner->getGame()->isAllyState(_owner->getPlayerType(), e->getPlayerType()))
			continue;

		Vec2 entityPos = e->getWorldPosition();
		if (entityPos.distance(pos) < maintainDistance)
		{
			avoidMove += -entityPos;
		}
	}

	InputMoveBegin moveBegin(_owner, avoidMove.getNormalized());
	moveBegin.execute();
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalAvoidCollision::process()
{
	// If status is INACTIVE, call activate()
	if (isInactive())
		activate();

	std::chrono::duration<double> endTime = std::chrono::system_clock::now().time_since_epoch();

	// Wait for equipping item.
	if ((endTime - _startTime).count() > 0.1f)
	{
		setGoalStatus(GoalStatus::COMPLETED);
		return getGoalStatus();
	}

	processSubgoals();
	
	return GoalStatus::ACTIVE;
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalAvoidCollision::terminate()
{
	InputMoveEnd moveEnd(_owner);
	moveEnd.execute();
}


int GoalAvoidCollision::evaluate(HumanBase* const owner)
{
	float maintainDistance = 220.0f;
	Vec2 pos = _owner->getWorldPosition();
	auto entities = _owner->getGame()->getNeighborsOnMove(pos, maintainDistance);

	int weight = 0;
	for (auto e : entities)
	{
		if (e->getTag() == _owner->getTag())
			continue;

		if (!isMasked(e->getFamilyMask(), FamilyMask::HUMAN_BASE))
			continue;

		if (_owner->getGame()->isAllyState(_owner->getPlayerType(), e->getPlayerType()))
			continue;

		if (e->getWorldPosition().distance(pos) < maintainDistance)
		{
			weight = 500;
			break;
		}
	}

	return weight;
}