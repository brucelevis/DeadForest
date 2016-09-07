#include "GoalMainAttack.h"
#include "AbstTargetingSystem.h"
#include "HumanBase.hpp"
#include "InputAttackBegin.hpp"
#include "InputAttackEnd.hpp"
#include "InputBezelBegin.hpp"
#include "InputBezelEnd.hpp"
#include "StateMachine.hpp"

using namespace realtrick::client;
USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalMainAttack::GoalMainAttack(HumanBase* owner, cocos2d::Vec2 target)
	:
	GoalBase(owner),
	_target(target)
{
	setGoalType(GoalType::GOAL_MAIN_ATTACK);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalMainAttack::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	_startTime = std::chrono::system_clock::now().time_since_epoch();


	if (_owner->getTargetSys()->isTargetPresent())
	{
		if (_owner->getTargetSys()->isTargetAttackable())
		{
			cocos2d::Vec2 heading = (_target - _owner->getWorldPosition()).getNormalized();

			InputBezelBegin bezelBegin(_owner, heading);
			bezelBegin.execute();

			InputAttackBegin attackBegin(_owner);
			attackBegin.execute();

			//InputMoveBegin moveBegin(_owner, (_position - _owner->getWorldPosition()).getNormalized());
			//moveBegin.execute();
		}
		else
		{
			setGoalStatus(GoalStatus::FAILED);
		}
	}
	else
	{
		setGoalStatus(GoalStatus::FAILED);
	}
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalMainAttack::process()
{
	//if status is INACTIVE, call activate()
	if (isInactive())
		activate();

	std::chrono::duration<double> endTime = std::chrono::system_clock::now().time_since_epoch();

	if ( _owner->getAnimator()->getTimeToReachMaxFrame() + 0.5 < (endTime - _startTime).count())
	{
		setGoalStatus(GoalStatus::COMPLETED);
		return getGoalStatus();
	}
	
	setGoalStatus(GoalStatus::ACTIVE);
	return getGoalStatus();
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalMainAttack::terminate()
{
	InputBezelEnd bezelEnd(_owner);
	bezelEnd.execute();

	InputAttackEnd attackEnd(_owner);
	attackEnd.execute();

	setGoalStatus(GoalStatus::COMPLETED);
}


