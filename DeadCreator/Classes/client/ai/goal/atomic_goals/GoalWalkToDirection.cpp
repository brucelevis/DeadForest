#include "GoalWalkToDirection.hpp"
#include "AbstTargetingSystem.hpp"
#include "SensoryMemory.hpp"
#include "HumanBase.hpp"
#include "InventoryData.hpp"
#include "InputBezelBegin.hpp"
#include "InputBezelEnd.hpp"
#include "InputMoveBegin.hpp"
#include "InputMoveEnd.hpp"
#include "StateMachine.hpp"

using namespace realtrick::client;
using namespace std::chrono;
USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalWalkToDirection::GoalWalkToDirection( HumanBase* owner, const cocos2d::Vec2& direction, float walkTime) :
GoalBase(owner),
_direction(direction),
_walkTime(walkTime)
{
    setGoalName("walk to direction");
    setGoalType(GoalType::WALK_TO_DIRECTION);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------
void GoalWalkToDirection::activate()
{
    setGoalStatus(GoalStatus::ACTIVE);
    _startTime = system_clock::now().time_since_epoch();
    
    InputMoveBegin moveBegin(_owner, _direction);
    moveBegin.execute();
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalWalkToDirection::process()
{
    if (isInactive())
        activate();
    
    duration<double> endTime =
    system_clock::now().time_since_epoch();
    
    if (_walkTime < (endTime - _startTime).count())
        setGoalStatus(GoalStatus::COMPLETED);
    
    return getGoalStatus();
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalWalkToDirection::terminate()
{
    InputMoveEnd moveEnd(_owner);
    moveEnd.execute();
    
    setGoalStatus(GoalStatus::COMPLETED);
}


