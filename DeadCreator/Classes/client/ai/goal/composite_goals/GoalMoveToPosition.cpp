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


GoalMoveToPosition::GoalMoveToPosition(
	HumanBase* const owner,
	cocos2d::Vec2 pos)
	:
	GoalCompositeBase(owner),
	_destination(pos)
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

	if (!_owner->getPathPlanner()->requestPathToPosition(_destination))
	{
		cocos2d::log("new GoalSeekToPosition");
		addSubgoal(new GoalSeekToPosition(_owner, _destination));
	}
	else
	{
		addSubgoal(new GoalFollowPath(_owner, _owner->getPathPlanner()->getPath()));
	}
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
		cocos2d::log("GoalMoveToPosition FAILED");
	}

	return _goalStatus;
}

void GoalMoveToPosition::terminate()
{
	removeAllSubgoals();
}