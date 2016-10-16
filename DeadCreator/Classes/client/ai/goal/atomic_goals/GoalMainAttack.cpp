#include "GoalMainAttack.h"
#include "AbstTargetingSystem.h"
#include "SensoryMemory.h"
#include "HumanBase.hpp"
#include "InventoryData.hpp"
#include "InputAttackBegin.hpp"
#include "InputAttackEnd.hpp"
#include "InputBezelBegin.hpp"
#include "InputBezelEnd.hpp"
#include "InputMoveBegin.hpp"
#include "InputMoveEnd.hpp"
#include "StateMachine.hpp"

using namespace realtrick::client;
USING_NS_CC;

cocos2d::Vec2 GoalMainAttack::smartMoving(HumanBase* owner)
{
	HumanBase* target = nullptr;
	if (owner->getTargetSys()->isTargetPresent())
	{
		target = owner->getTargetSys()->getTarget();

		if (!(target->getTargetSys()->getTarget() == owner))
			return Vec2();

		return target->getMoving();
	}
	return owner->getMoving();
}

cocos2d::Vec2 GoalMainAttack::roughMoving(HumanBase* owner)
{
	HumanBase* target = nullptr;
	if (owner->getTargetSys()->isTargetPresent())
	{
		target = owner->getTargetSys()->getTarget();
		return (target->getWorldPosition() - owner->getWorldPosition()).getNormalized();
	}
	return owner->getMoving();
}

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalMainAttack::GoalMainAttack(
	HumanBase* owner,
	const cocos2d::Vec2& target,
	std::function<cocos2d::Vec2(HumanBase*)> movingStrategy)
	:
	GoalBase(owner),
	_target(target),
	_movingStrategy(movingStrategy)
{
	setGoalType(GoalType::MAIN_ATTACK);
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

			InputMoveBegin moveBegin(_owner, _movingStrategy(_owner));
			moveBegin.execute();
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
	if (isInactive())
		activate();

	std::chrono::duration<double> endTime = std::chrono::system_clock::now().time_since_epoch();

	if ( 1.0f < (endTime - _startTime).count())
	{
		setGoalStatus(GoalStatus::COMPLETED);
		return getGoalStatus();
	}
	
	return getGoalStatus();
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalMainAttack::terminate()
{
	InputMoveEnd moveEnd(_owner);
	moveEnd.execute();

	InputBezelEnd bezelEnd(_owner);
	bezelEnd.execute();

	InputAttackEnd attackEnd(_owner);
	attackEnd.execute();

	setGoalStatus(GoalStatus::COMPLETED);
}


