#include "GoalRangeAttack.h"
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

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalRangeAttack::GoalRangeAttack(HumanBase* owner, const cocos2d::Vec2& target)
	:
	GoalBase(owner),
	_target(target),
	_attacked(false)
{
	setGoalType(GoalType::RANGE_ATTACK);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalRangeAttack::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	_startTime = std::chrono::system_clock::now().time_since_epoch();

	if (_owner->getTargetSys()->isTargetPresent())
	{
		cocos2d::Vec2 pos = _owner->getWorldPosition();
		cocos2d::Vec2 heading = (_target - pos).getNormalized();

		InputBezelBegin bezelBegin(_owner, heading);
		bezelBegin.execute();

		if (_owner->getTargetSys()->isTargetAttackable())
		{
			float attackRange = _owner->getSensoryMemory()->getAttackRange();

			if (_target.distance(pos) > attackRange)
			{
				InputMoveBegin moveBegin(_owner, (_target - pos).getNormalized());
				moveBegin.execute();
			}
			else
			{
				int dice = random(0, 100) % 2;
				if (dice == 0)
				{
					InputMoveBegin moveBegin(_owner, (_target - pos).getPerp().getNormalized());
					moveBegin.execute();
				}
				else
				{
					InputMoveBegin moveBegin(_owner, (pos - _target).getNormalized());
					moveBegin.execute();
				}
			}
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
GoalStatus GoalRangeAttack::process()
{
	//if status is INACTIVE, call activate()
	if (isInactive())
		activate();

	std::chrono::duration<double> endTime = std::chrono::system_clock::now().time_since_epoch();

	// Attack when bezel is ready
	if (!_attacked && 0.15f < (endTime - _startTime).count())
	{
		if (_owner->getTargetSys()->isTargetPresent())
		{
			if (_owner->getTargetSys()->isTargetAttackable())
			{
				InputAttackBegin attackBegin(_owner);
				attackBegin.execute();

				_attacked = true;
			}
		}
	}

	if ( 1.0f < (endTime - _startTime).count())
	{
		setGoalStatus(GoalStatus::COMPLETED);
		return getGoalStatus();
	}
	
	return getGoalStatus();
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalRangeAttack::terminate()
{
	InputBezelEnd bezelEnd(_owner);
	bezelEnd.execute();

	InputAttackEnd attackEnd(_owner);
	attackEnd.execute();

	InputMoveEnd moveEnd(_owner);
	moveEnd.execute();

	setGoalStatus(GoalStatus::COMPLETED);
}


