//
//  GoalReadyToFight.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalReadyToFight.hpp"
#include "Goals.hpp"
using namespace realtrick::client;


GoalReadyToFight::GoalReadyToFight(HumanBase* owner) : GoalCompositeBase(owner)
{
    setGoalType(GoalType::READY_TO_FIGHT);
    _goalFindWeapon = new GoalFindWeapon(owner);
}


GoalReadyToFight::~GoalReadyToFight()
{
    CC_SAFE_DELETE(_goalFindWeapon);
}


void GoalReadyToFight::activate()
{
    setGoalStatus(GoalStatus::ACTIVE);
    
}


GoalStatus GoalReadyToFight::process()
{
    return getGoalStatus();
}


void GoalReadyToFight::terminate()
{
}









