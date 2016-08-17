//
//  GoalWander.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 10..
//
//

#include "GoalWander.hpp"
#include "InputCommands.hpp"
using namespace realtrick::client;


GoalWander::GoalWander(HumanBase* owner) : GoalBase(owner)
{
    setGoalType(GoalType::WANDER);
}


GoalWander::~GoalWander()
{
}


void GoalWander::activate()
{
    setGoalStatus(GoalStatus::ACTIVE);
    _start = std::chrono::system_clock::now().time_since_epoch();
    
    InputMoveBegin input(_owner, cocos2d::Vec2(cocos2d::random(-1.0f, 1.0f), cocos2d::random(-1.0f, 1.0f)).getNormalized());
    input.execute();
}


GoalStatus GoalWander::process()
{
    if ( getGoalStatus() == GoalStatus::INACTIVE ) activate();
    
    std::chrono::duration<double> interval = (std::chrono::system_clock::now().time_since_epoch() - _start);
    if ( interval.count() > 1.0 )
    {
        return GoalStatus::COMPLETED;
    }
    
    return getGoalStatus();
}


void GoalWander::terminate()
{
    InputMoveEnd input(_owner);
    input.execute();
}









