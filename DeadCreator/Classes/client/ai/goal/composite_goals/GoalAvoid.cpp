#include "GoalAvoid.hpp"
#include "GoalSeekToPosition.hpp"
#include "HumanBase.hpp"
#include "AbstTargetingSystem.hpp"
#include "SensoryMemory.hpp"
#include "InputMoveBegin.hpp"
#include "InputMoveEnd.hpp"

namespace
{
    float kMoveDistance = 150.0f;
}

using namespace realtrick::client;
using namespace realtrick;
USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalAvoid::GoalAvoid(HumanBase* owner) :
GoalCompositeBase(owner)
{
    setGoalName("avoid");
    setGoalType(GoalType::AVOID);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------
void GoalAvoid::activate()
{
    setGoalStatus(GoalStatus::ACTIVE);
    if (_owner->getTargetSys()->isTargetPresent())
    {
        Vec2 avoidMove(_owner->getSensoryMemory()->avoidingEnemiesVector(_owner->getWorldPosition(), _owner->getHeading()));
        addSubgoal(new GoalSeekToPosition(_owner, _owner->getWorldPosition() + avoidMove * kMoveDistance));
    }
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalAvoid::process()
{
    // If status is INACTIVE, call activate()
    if (isInactive())
        activate();
    
    if (getGoalStatus() == GoalStatus::COMPLETED ||
        getGoalStatus() == GoalStatus::FAILED)
        return GoalStatus::COMPLETED;
    
    processSubgoals();
    
    return GoalStatus::ACTIVE;
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalAvoid::terminate()
{
    InputMoveEnd moveEnd(_owner);
    moveEnd.execute();
}


