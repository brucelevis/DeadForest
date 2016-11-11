#include "GoalShake.hpp"
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
GoalShake::GoalShake( HumanBase* owner, const cocos2d::Vec2& movingDirection, const cocos2d::Vec2& shakingAxis, float shakeTime) :
GoalBase(owner),
_movingDirection(movingDirection),
_shakingAxis(shakingAxis),
_shakeTime(shakeTime)
{
    setGoalName("shake");
    setGoalType(GoalType::SHAKE);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------
void GoalShake::activate()
{
    setGoalStatus(GoalStatus::ACTIVE);
    _startTime = system_clock::now().time_since_epoch();
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalShake::process()
{
    if (isInactive())
        activate();
    
    if (!_owner->getTargetSys()->isTargetPresent())
        return GoalStatus::COMPLETED;
    
    duration<double> endTime = system_clock::now().time_since_epoch();
    duration<double> delayed = endTime - _startTime;
    
    float term = _shakeTime / (float)4;
    for (int i = 0; i < 4; i++)
    {
        if (delayed.count() > _shakeTime * (float)i * term &&
            delayed.count() < _shakeTime * (float)(i + 1)* term)
        {
            float sign = 1.0f;
            if (i % 2 == 0)
                sign = -sign;
            
            InputMoveBegin moveBegin(_owner, (_movingDirection + sign * _shakingAxis.getPerp()).getNormalized());
            moveBegin.execute();
        }
    }
    
    if (_shakeTime < delayed.count())
    {
        setGoalStatus(GoalStatus::COMPLETED);
    }
    
    return getGoalStatus();
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalShake::terminate()
{
    InputMoveEnd moveEnd(_owner);
    moveEnd.execute();
    
    setGoalStatus(GoalStatus::COMPLETED);
}


