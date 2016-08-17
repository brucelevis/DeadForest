//
//  GoalFollowPath.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalFollowPath.hpp"
using namespace realtrick::client;


GoalFollowPath::GoalFollowPath(HumanBase* owner) : GoalCompositeBase(owner)
{
    setGoalType(GoalType::FOLLOW_PATH);
}


GoalFollowPath::~GoalFollowPath()
{
}


void GoalFollowPath::activate()
{
    
}


GoalStatus GoalFollowPath::process()
{
    return getGoalStatus();
}


void GoalFollowPath::terminate()
{
    
}









