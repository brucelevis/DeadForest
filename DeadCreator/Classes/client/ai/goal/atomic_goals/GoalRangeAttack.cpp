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
	_target(target)
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
		if (_owner->getTargetSys()->isTargetAttackable())
		{
			cocos2d::Vec2 heading = (_target - _owner->getWorldPosition()).getNormalized();

			InputBezelBegin bezelBegin(_owner, heading);
			bezelBegin.execute();

			InputAttackBegin attackBegin(_owner);
			attackBegin.execute();

			int amount = 0;
			if (_owner->getEquipedWeapon() != nullptr)
			{
				EntityType bulletType = _owner->getEquipedWeapon()->getBulletType();
				amount = _owner->getInventoryData()->getItemAmount(bulletType);
				cocos2d::log("range %d %d", _owner->getEquipedWeapon()->getReservedBullets(), amount);
			}
			
			// 여기 방향을 유지하며 뒤로 움직일 방법이 필요함
			Vec2 pos = _owner->getWorldPosition();
			//if (_target.distance(pos) < (float)_owner->getSensoryMemory()->getAttackRange() * 0.75f)
			//{
				InputMoveBegin moveBegin(_owner, (_target - pos).getNormalized());
				moveBegin.execute();
			//}
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


