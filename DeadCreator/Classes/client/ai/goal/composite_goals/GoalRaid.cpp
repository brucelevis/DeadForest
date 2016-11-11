//
//  GoalTraverseEdge.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalRaid.hpp"
#include "GoalMoveToPosition.hpp"
#include "GraphEdgeTypes.hpp"
#include "InputCommands.hpp"
#include "PathEdge.hpp"
#include "HumanBase.hpp"
#include "PathPlanner.hpp"
#include "AbstTargetingSystem.hpp"
#include "GoalMainAttack.hpp"
#include "GoalRangeAttack.hpp"
#include "GoalHuntTarget.hpp"
#include "SensoryMemory.hpp"
#include "InventoryData.hpp"



using namespace realtrick;
using namespace realtrick::client;
USING_NS_CC;

GoalRaid::GoalRaid( HumanBase* const owner, std::function<cocos2d::Vec2(HumanBase*)> movingStrategy) :
GoalCompositeBase(owner),
_arrived(false),
_movingStrategy(movingStrategy)
{
    setGoalName("raid");
    setGoalType(GoalType::RAID);
}


//------------------------------- Activate ------------------------------------
//-----------------------------------------------------------------------------
void GoalRaid::activate()
{
    _arrived = false;
    setGoalStatus(GoalStatus::ACTIVE);
    
    // if this goal is reactivated then there may be some existing subgoals that
    // must be removed
    removeAllSubgoals();
}




//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalRaid::process()
{
    if (isInactive())
        activate();
    
    // it is possible for a bot's target to die whilst this goal is active so we
    // must test to make sure the bot always has an active target
    if (!_owner->getTargetSys()->isTargetPresent() ||
        !_owner->getTargetSys()->isTargetWithinFOV())
    {
        return GoalStatus::COMPLETED;
    }
    
    auto target = _owner->getTargetSys()->getTarget();
    cocos2d::Vec2 heading = (target->getWorldPosition() - _owner->getWorldPosition()).getNormalized();
    InputMoveBegin moveBegin(_owner, heading);
    moveBegin.execute();
    
    // process the subgoals
    _goalStatus = processSubgoals();
    
    if (!_arrived && _owner->getTargetSys()->isTargetAttackable())
    {
        _arrived = true;
        setGoalStatus(GoalStatus::ACTIVE);
        addSubgoal(
                   new GoalMainAttack(
                                      _owner,
                                      _owner->getTargetSys()->getTarget()->getWorldPosition(),
                                      _movingStrategy));
        
        InputMoveEnd moveEnd(_owner);
        moveEnd.execute();
    }
    
    if (_arrived)
        return _goalStatus;
    else
        return GoalStatus::ACTIVE;
}


void GoalRaid::terminate()
{
    removeAllSubgoals();
    InputMoveEnd moveEnd(_owner);
    moveEnd.execute();
}
