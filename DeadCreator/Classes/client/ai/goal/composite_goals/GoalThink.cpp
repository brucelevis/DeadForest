//
//  GoalThink.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 10..
//
//

#include "GoalThink.hpp"
#include "Goals.hpp"
#include "HumanBase.hpp"
using namespace realtrick::client;


GoalThink::GoalThink(HumanBase* owner) : GoalCompositeBase(owner),
_immediateGoal(nullptr)
{
    setGoalName("think");
    setGoalType(GoalType::THINK);
}


GoalThink::~GoalThink()
{
    for( auto& goal : _goalEntry ) CC_SAFE_DELETE(goal);
    _goalEntry.clear();
}


void GoalThink::activate()
{
    if ( _goalEntry.empty() ) return;
    
	GoalBase* bestGoal = nullptr;
	int weight = -1;

	for (auto e : _goalEntry)
	{
		int entWeight = e->getWeight();
		if (entWeight > weight)
		{
			bestGoal = e;
			weight = entWeight;
		}
	}

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
    for(auto& goal : _subGoals )
        goal->terminate();
    _subGoals.clear();
    
    if ( _immediateGoal )
    {
        _immediateGoal->terminate();
        CC_SAFE_DELETE(_immediateGoal);
    }
    
    setGoalStatus(GoalStatus::COMPLETED);
}


void GoalThink::executeGoal(GoalBase* immediateGoal)
{
    if ( _immediateGoal )
    {
        _immediateGoal->terminate();
        CC_SAFE_DELETE(_immediateGoal);
    }
    
    _immediateGoal = immediateGoal;
    
    addSubgoal(immediateGoal);
    setGoalStatus(GoalStatus::ACTIVE);
}








