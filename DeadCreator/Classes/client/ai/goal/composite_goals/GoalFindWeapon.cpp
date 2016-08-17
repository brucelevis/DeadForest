//
//  GoalFindWeapon.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalFindWeapon.hpp"
using namespace realtrick::client;


GoalFindWeapon::GoalFindWeapon(HumanBase* owner) : GoalCompositeBase(owner)
{
    setGoalType(GoalType::FIND_WEAPON);
}


GoalFindWeapon::~GoalFindWeapon()
{
}


void GoalFindWeapon::activate()
{
    setGoalStatus(GoalStatus::ACTIVE);
}


GoalStatus GoalFindWeapon::process()
{
    if ( isInactive() ) activate();
    
    auto subGoalStatus = processSubgoals();
    if ( subGoalStatus == GoalStatus::FAILED )
    {
        activate();
    }
    
    return getGoalStatus();
}


void GoalFindWeapon::terminate()
{
}









