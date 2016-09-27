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
    setGoalType(GoalType::THINK);
}


GoalThink::~GoalThink()
{
    for( auto& goal : _goalEntry ) CC_SAFE_DELETE(goal);
    _goalEntry.clear();
}


void GoalThink::activate()
{
    if ( _goalEntry.empty() ) return ;
    
    auto bestGoal = 
		*std::max_element(std::begin(_goalEntry), std::end(_goalEntry), 
			[](GoalBase* g1, GoalBase* g2) 
	{
        return (g1->getWeight() < g2->getWeight());
    });
    addSubgoal(bestGoal);
    setGoalStatus(GoalStatus::ACTIVE);
}


GoalStatus GoalThink::process()
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









