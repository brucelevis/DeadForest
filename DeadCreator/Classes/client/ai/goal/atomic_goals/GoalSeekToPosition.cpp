//
//  GoalTraverseEdge.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalSeekToPosition.hpp"
#include "InputCommands.hpp"
#include "GraphEdgeTypes.h"
#include "PathPlanner.h"
#include "PathEdge.h"


using namespace realtrick;
using namespace realtrick::client;



//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalSeekToPosition::GoalSeekToPosition(HumanBase* owner, cocos2d::Vec2 target)
	:
	GoalBase(owner),
	_position(target),
	_time_expected(0.0)
{
	setGoalType(GoalType::GOAL_SEEK_TO_POS);
}





//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalSeekToPosition::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	//record the time the bot starts this goal
	_start = std::chrono::system_clock::now().time_since_epoch();

	//This value is used to determine if the bot becomes stuck 
	_time_expected = std::chrono::duration<double>(
		_owner->getPathPlanner()->calculateTimeToReachPosition(_position));

	//factor in a margin of error for any reactive behavior
	std::chrono::duration<double> margin_of_error(0.02);

	_time_expected += margin_of_error;

	InputMoveBegin moveBegin(_owner, (_position - _owner->getWorldPosition()).getNormalized());
	moveBegin.execute();
}






GoalStatus GoalSeekToPosition::process()
{
	//if status is INACTIVE, call activate()
	if (isInactive())
		activate();

	//if the bot has become stuck return failure
	if (isStuck())
	{
		setGoalStatus(GoalStatus::FAILED);
	}

	//if the bot has reached the end of the edge return COMPLETED
	else
	{
		if (Circle(_owner->getWorldPosition(), _owner->getBoundingRadius()).containPoint(_position))
		{
			setGoalStatus(GoalStatus::COMPLETED);
		}
	}

    return getGoalStatus();
}


void GoalSeekToPosition::terminate()
{
	InputMoveEnd moveEnd(_owner);
	moveEnd.execute();
}



//--------------------------- isStuck --------------------------------------
//
//  returns true if the bot has taken longer than expected to reach the 
//  currently ACTIVE waypoint
//-----------------------------------------------------------------------------
bool GoalSeekToPosition::isStuck()const
{
	auto TimeTaken = std::chrono::system_clock::now().time_since_epoch() - _start;

	if (TimeTaken > _time_expected)
	{
		cocos2d::log("BOT IS STUCK!! (%f %f)", _owner->getWorldPosition().x, _owner->getWorldPosition().y);

		return true;
	}

	return false;
}
