//
//  GoalTraverseEdge.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalTraverseEdge.hpp"
#include "InputCommands.hpp"
#include "GraphEdgeTypes.hpp"
#include "PathPlanner.hpp"

using namespace realtrick;
using namespace realtrick::client;
using namespace std::chrono;
USING_NS_CC;


GoalTraverseEdge::GoalTraverseEdge(HumanBase* owner, PathEdge edge)
:
GoalBase(owner),
_edge(edge),
_time_expected(0.0)
{
    setGoalType(GoalType::TRAVERSE_EDGE);
}


void GoalTraverseEdge::activate()
{
    setGoalStatus(GoalStatus::ACTIVE);
    
    //the edge behavior flag may specify a type of movement that necessitates a
    //change in the bot's max possible speed as it follows this edge
    switch (_edge.getBehavior())
    {
        case NavGraphEdge::SWIM:
        {
            break;
        }
        case NavGraphEdge::CRAWL:
        {
            break;
        }
        default: break;
    }
    
    //record the time the bot starts this goal
    _start = system_clock::now().time_since_epoch();
    
    
    //calculate the expected time required to reach the this waypoint. This value
    //is used to determine if the bot becomes stuck
    _time_expected = duration<double>(_owner->getPathPlanner()->calculateTimeToReachPosition(_edge.getDestination()));
    
    //factor in a margin of error for any reactive behavior
    duration<double> margin_of_error(0.01);
    _time_expected += margin_of_error;
    
    InputMoveBegin moveBegin(_owner, (_edge.getDestination() - _owner->getWorldPosition()).getNormalized());
    moveBegin.execute();
}


GoalStatus GoalTraverseEdge::process()
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
        if (_owner->getWorldPosition().distance(_edge.getDestination()) < _owner->getBoundingRadius() * 4.0f)
        {
            setGoalStatus(GoalStatus::COMPLETED);
        }
    }
    
    return getGoalStatus();
}


void GoalTraverseEdge::terminate()
{
    InputMoveEnd moveEnd(_owner);
    moveEnd.execute();
}

realtrick::PathEdge GoalTraverseEdge::getEdge() const { return _edge; }


//--------------------------- isStuck --------------------------------------
//
//  returns true if the bot has taken longer than expected to reach the
//  currently ACTIVE waypoint
//-----------------------------------------------------------------------------
bool GoalTraverseEdge::isStuck()const
{
    duration<double> endTime = system_clock::now().time_since_epoch();
    duration<double> timeTaken = endTime - _start;
    
    if (timeTaken > _time_expected)
        return true;
    
    return false;
}


