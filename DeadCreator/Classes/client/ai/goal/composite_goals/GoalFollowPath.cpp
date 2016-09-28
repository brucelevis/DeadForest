//
//  GoalTraverseEdge.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalFollowPath.hpp"
#include "GoalTraverseEdge.hpp"
#include "GraphEdgeTypes.h"
#include "InputCommands.hpp"
#include "PathEdge.h"


using namespace realtrick;
using namespace realtrick::client;


//------------------------------ ctor -----------------------------------------
//-----------------------------------------------------------------------------
GoalFollowPath::GoalFollowPath(
	HumanBase* const owner,
	std::list<realtrick::PathEdge> path)
	:
	GoalCompositeBase(owner),
	_path(path)
{
	setGoalType(GoalType::GOAL_FOLLOW_PATH);
}



//------------------------------ activate -------------------------------------
//-----------------------------------------------------------------------------
void GoalFollowPath::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	if (_path.empty())
		return;

	//get a reference to the next edge
	realtrick::PathEdge edge = _path.front();

	//remove the edge from the path
	_path.pop_front();

	//some edges specify that the bot should use a specific behavior when
	//following them. This switch statement queries the edge behavior flag and
	//adds the appropriate goals/s to the subgoal list.

	switch (edge.getBehavior())
	{
	case NavGraphEdge::NORMAL:
	{
		addSubgoal(new GoalTraverseEdge(_owner, edge, _path.empty()));
	}

	break;

	case NavGraphEdge::GOES_THROUGH_DOOR:
	{

		//also add a goal that is able to handle opening the door
		//addSubgoal(new Goal_NegotiateDoor(_owner, edge, _path.empty()));
	}

	break;

	case NavGraphEdge::JUMP:
	{
		//add subgoal to JUMP along the edge
	}

	break;

	case NavGraphEdge::GRAPPLE:
	{
		//add subgoal to GRAPPLE along the edge
	}

	break;

	default:

		throw std::runtime_error("<GoalFollowPath::activate>: Unrecognized edge type");
	}
}

//-------------------------- process ------------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalFollowPath::process()
{
	//if status is INACTIVE, call activate()
	if (isInactive())
		activate();

	_goalStatus = processSubgoals();
	//if there are no subgoals present check to see if the path still has edges.
	//remaining. If it does then call activate to grab the next edge.

	if (_goalStatus == GoalStatus::COMPLETED && !_path.empty())
	{
		activate();
	}

	return _goalStatus;
}

void GoalFollowPath::terminate()
{}






















