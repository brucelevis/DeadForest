//
//  GoalHuntTarget.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalHuntTarget.hpp"
#include "Goals.hpp"
#include "HumanBase.hpp"
#include "AbstTargetingSystem.hpp"
#include "GoalMoveToPosition.hpp"
#include "SensoryMemory.hpp"
using namespace realtrick::client;


GoalHuntTarget::GoalHuntTarget(HumanBase* owner)  :
GoalCompositeBase(owner),
_last_visit_visible_pos_tried(false)
{
    setGoalName("hunt target");
    setGoalType(GoalType::HUNT_TARGET);
}

//------------------------------ activate -------------------------------------
//-----------------------------------------------------------------------------
void GoalHuntTarget::activate()
{
    setGoalStatus(GoalStatus::ACTIVE);
    
    //if this goal is reactivated then there may be some existing subgoals that
    //must be removed
    removeAllSubgoals();
    
    //it is possible for the target to die whilst this goal is active so we
    //must test to make sure the bot always has an active target
    if (_owner->getTargetSys()->isTargetPresent())
    {
        //grab a local copy of the last recorded position (LRP) of the target
        const cocos2d::Vec2 lrp = _owner->getTargetSys()->getLastRecordedPosition();
        
        //if the bot has reached the LRP and it still hasn't found the target
        //it starts to search by using the explore goal to move to random
        //map locations
        if (Circle(_owner->getWorldPosition(), _owner->getSensoryMemory()->getAttackRange()).containPoint(lrp))
        {
            setGoalStatus(GoalStatus::COMPLETED);
        }
        
        //else move to the LRP
        else
        {
            addSubgoal(new GoalMoveToPosition(_owner, lrp));
        }
    }
    
    //if their is no active target then this goal can be removed from the queue
    else
    {
        setGoalStatus(GoalStatus::COMPLETED);
    }
    
}

GoalStatus GoalHuntTarget::process()
{
    if ( isInactive() ) activate();
    
    auto subGoalStatus = processSubgoals();
    setGoalStatus(subGoalStatus);
    
    // if target is in view this goal is satisfied
    if (_owner->getTargetSys()->isTargetAttackable())
    {
        setGoalStatus(GoalStatus::COMPLETED);
    }
    
    return getGoalStatus();
}


void GoalHuntTarget::terminate()
{
    removeAllSubgoals();
}

