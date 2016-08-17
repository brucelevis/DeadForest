//
//  GoalHuntTarget.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalHuntTarget.hpp"
#include "Goals.hpp"
using namespace realtrick::client;


GoalHuntTarget::GoalHuntTarget(HumanBase* owner) : GoalCompositeBase(owner)
{
    setGoalType(GoalType::HUNT_TARGET);
    
    _goalReadyToFight = new GoalReadyToFight(owner);
}


GoalHuntTarget::~GoalHuntTarget()
{
    CC_SAFE_DELETE(_goalReadyToFight);
}


void GoalHuntTarget::activate()
{
    setGoalStatus(GoalStatus::ACTIVE);
    
    addSubgoal(_goalReadyToFight);
}


GoalStatus GoalHuntTarget::process()
{
    if ( isInactive() ) activate();
    
    auto subGoalStatus = processSubgoals();
    if ( subGoalStatus == GoalStatus::FAILED )
    {
        activate();
    }
    
    return getGoalStatus();
}


void GoalHuntTarget::terminate()
{
}









