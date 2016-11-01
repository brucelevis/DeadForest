//
//  GoalWander.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 8. 10..
//
//

#include "GoalWander.hpp"
#include "InputCommands.hpp"

USING_NS_CC;
using namespace realtrick::client;
using namespace std::chrono;


GoalWander::GoalWander(HumanBase* owner) : GoalBase(owner)
{
    setGoalType(GoalType::WANDER);
}


GoalWander::~GoalWander()
{}


void GoalWander::activate()
{
    setGoalStatus(GoalStatus::ACTIVE);
    _start = system_clock::now().time_since_epoch();
    
    InputMoveBegin input(_owner, Vec2(random(-1.0f, 1.0f), random(-1.0f, 1.0f)).getNormalized());
    input.execute();
}


GoalStatus GoalWander::process()
{
    if ( getGoalStatus() == GoalStatus::INACTIVE ) activate();
    
    duration<double> interval = 
		(system_clock::now().time_since_epoch() - _start);
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









