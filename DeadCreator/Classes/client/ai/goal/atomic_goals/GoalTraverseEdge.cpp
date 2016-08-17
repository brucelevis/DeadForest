//
//  GoalTraverseEdge.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalTraverseEdge.hpp"
#include "InputCommands.hpp"
using namespace realtrick::client;


GoalTraverseEdge::GoalTraverseEdge(HumanBase* owner) : GoalBase(owner)
{
    setGoalType(GoalType::TRAVERSE_EDGE);
}


GoalTraverseEdge::~GoalTraverseEdge()
{
}


void GoalTraverseEdge::activate()
{
    setGoalStatus(GoalStatus::ACTIVE);
}


GoalStatus GoalTraverseEdge::process()
{
    return getGoalStatus();
}


void GoalTraverseEdge::terminate()
{
}









