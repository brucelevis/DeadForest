//
//  GoalTraverseEdge.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalMoveToPosition.hpp"
#include "GoalFollowPath.hpp"
#include "GoalSeekToPosition.hpp"
#include "GraphEdgeTypes.h"
#include "InputCommands.hpp"
#include "PathEdge.h"
#include "HumanBase.hpp"
#include "PathPlanner.h"

using namespace realtrick;
using namespace realtrick::client;
USING_NS_CC;


GoalMoveToPosition::GoalMoveToPosition(
	HumanBase* const owner,
	Vec2 pos,
	std::shared_ptr<ArrivingData> arrivingData)
	:
	GoalCompositeBase(owner),
	_destination(pos),
	_arrivingData(arrivingData)
{
	setGoalType(GoalType::MOVE_TO_POS);
}


//------------------------------- activate ------------------------------------
//-----------------------------------------------------------------------------
void GoalMoveToPosition::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	//make sure the subgoal list is clear.
	removeAllSubgoals();

	const auto& walls = _owner->getGame()->getNeighborSimpleWalls(_destination, 1.0f);
	bool canReach = true;

	for (auto& wall : walls)
	{
		if (wall.containPoint(_destination))
		{
			canReach = false;
			break;
		}
	}

	if (!canReach)
	{
		setGoalStatus(GoalStatus::COMPLETED);
		return;
	}

	if (!_owner->getPathPlanner()->requestPathToPosition(_destination))
		addSubgoal(new GoalSeekToPosition(_owner, _destination, _arrivingData));

	else
		addSubgoal(new GoalFollowPath(_owner, _owner->getPathPlanner()->getPath()));
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalMoveToPosition::process()
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
		log("GoalMoveToPosition FAILED");
	}

	return _goalStatus;
}

void GoalMoveToPosition::terminate()
{
	removeAllSubgoals();
}