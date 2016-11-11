//
//  GoalTraverseEdge.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalSeekToPosition.hpp"
#include "InputCommands.hpp"
#include "GraphEdgeTypes.hpp"
#include "PathPlanner.hpp"
#include "PathEdge.hpp"


using namespace realtrick;
using namespace realtrick::client;
using namespace std::chrono;
USING_NS_CC;


GoalSeekToPosition::GoalSeekToPosition( HumanBase* owner, Vec2 target, std::shared_ptr<ArrivingData> arrivingData) :
GoalBase(owner),
_destination(target),
_time_expected(0.0f),
_arrivingData(arrivingData)
{
    setGoalName("seek to position");
    setGoalType(GoalType::SEEK_TO_POS);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------
void GoalSeekToPosition::activate()
{
    setGoalStatus(GoalStatus::ACTIVE);
    
    // Record the time the bot starts this goal
    _start = system_clock::now().time_since_epoch();
    
    // This value is used to determine if the bot becomes stuck
    _time_expected = duration<double>(
                                      _owner->getPathPlanner()->calculateTimeToReachPosition(_destination));
    
    // Factor in a margin of error for any reactive behavior
    duration<double> margin_of_error(0.02);
    
    _time_expected += margin_of_error;
    
    InputMoveBegin moveBegin(_owner, (_destination - _owner->getWorldPosition()).getNormalized());
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
        Vec2 pos = _owner->getWorldPosition();
        float radius = _owner->getBoundingRadius();
        
        Vec2 arriveHeading;
        float startWalkRange;
        float arriveRange;
        
        if (_arrivingData == nullptr)
        {
            arriveHeading = _owner->getHeading();
            startWalkRange = radius * 2.5f;
            arriveRange = radius * 0.5f;
        }
        else
        {
            arriveHeading = _arrivingData->arriveHeading;
            startWalkRange = _arrivingData->startWalkRange;
            arriveRange = _arrivingData->arriveRange;
        }
        
        if (pos.distance(_destination) < startWalkRange)
        {
            InputBezelBegin bezelBegin(_owner, arriveHeading);
            bezelBegin.execute();
            
            if (pos.distance(_destination) < arriveRange)
                setGoalStatus(GoalStatus::COMPLETED);
        }
    }
    
    return getGoalStatus();
}


void GoalSeekToPosition::terminate()
{
    InputMoveEnd moveEnd(_owner);
    moveEnd.execute();
    
    InputBezelEnd bezelEnd(_owner);
    bezelEnd.execute();
}



//--------------------------- isStuck --------------------------------------
//
//  returns true if the bot has taken longer than expected to reach the
//  currently ACTIVE waypoint
//-----------------------------------------------------------------------------
bool GoalSeekToPosition::isStuck()const
{
    duration<double> endTime
    = system_clock::now().time_since_epoch();
    
    duration<double> timeTaken = endTime - _start;
    
    if (timeTaken > _time_expected)
        return true;
    
    return false;
}
