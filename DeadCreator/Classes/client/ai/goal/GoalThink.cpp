//
//  GoalThink.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 10..
//
//

#include "GoalThink.hpp"
#include "Goals.hpp"
using namespace realtrick::client;


GoalThink::GoalThink(HumanBase* owner) : GoalCompositeBase(owner)
{
    /*
     
     create sub goals
     
     */
}


GoalThink::~GoalThink()
{
}


void GoalThink::activate()
{
    addSubgoal(new GoalWander(_owner));
    setGoalStatus(GoalStatus::ACTIVE);
}


GoalBase::GoalStatus GoalThink::process()
{
    if ( isInactive() ) activate();
    
    auto subGoalStatus = processSubgoals();
    if ( subGoalStatus == GoalStatus::COMPLETED || subGoalStatus == GoalStatus::FAILED )
    {
        setGoalStatus(GoalStatus::INACTIVE);
    }
    
    return getGoalStatus();
}


void GoalThink::terminate()
{
    
}









