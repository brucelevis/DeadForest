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
#include "GoalNetwork.h"
#include "Mat3.hpp"

namespace
{
	const float kDelayForCompleteAttack = 0.25f;
	const float kSafeMaintainRangeRatio = 0.7f;
}

using namespace realtrick::client;
using namespace std::chrono;
USING_NS_CC;

Vec2 GoalRangeAttack::smartMoving(HumanBase* owner)
{
	HumanBase* target = nullptr;
	if (owner->getTargetSys()->isTargetPresent())
	{
		target = owner->getTargetSys()->getTarget();

		Vec2 ownerPos = owner->getWorldPosition();
		Vec2 targetPos = target->getWorldPosition();

		if (0 < target->getMoving().dot((targetPos - ownerPos).getNormalized()))
		{
			if (owner->getTargetSys()->isTargetAttackable())
			{
				if (owner->getSensoryMemory()->getAttackRange() * kSafeMaintainRangeRatio >
					targetPos.distance(ownerPos))
					return -(targetPos - ownerPos).getNormalized();
				else
					return Vec2();
			}
		}

		return target->getMoving();
	}
	return Vec2();
}

Vec2 GoalRangeAttack::dealCrowdMoving(HumanBase* owner)
{
	cocos2d::Vec2 avoidMove(owner->getSensoryMemory()->avoidingEnemiesVector(owner));
	return avoidMove.getNormalized();
}


Vec2 GoalRangeAttack::makeFormationMoving(HumanBase* owner)
{
	HumanBase* leader = owner->getTargetSys()->getLeader();

	if (leader == nullptr)
		return smartMoving(owner);
	
	int idx = leader->getTargetSys()->queryFollowerIndex(owner);

	// Has no leader
	if (idx < 0)
		return smartMoving(owner);

	Vec2 align = GoalNetwork::queryFormationPos(owner, leader, idx);

	if (align.distance(owner->getWorldPosition()) < owner->getBoundingRadius() * 2)
		return Vec2();
	else
		return (align - owner->getWorldPosition()).getNormalized();
}

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalRangeAttack::GoalRangeAttack(
	HumanBase* owner,
	const cocos2d::Vec2& target,
	std::function<cocos2d::Vec2(HumanBase*)> movingStrategy)
	:
	GoalBase(owner),
	_target(target),
	_attacked(false),
	_movingStrategy(movingStrategy)
{
	setGoalType(GoalType::RANGE_ATTACK);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalRangeAttack::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	_startTime = system_clock::now().time_since_epoch();

	if (_owner->getTargetSys()->isTargetPresent())
	{
		Vec2 targetPos = _owner->getTargetSys()->getTarget()->getWorldPosition();
		Vec2 ownerPos = _owner->getWorldPosition();
		Vec2 aim = (targetPos - ownerPos).getNormalized();

		InputBezelBegin bezelBegin(_owner, aim);
		bezelBegin.execute();

		if (_owner->getTargetSys()->isTargetAttackable())
		{
			float attackRange = _owner->getSensoryMemory()->getAttackRange();

			if (targetPos.distance(ownerPos) > attackRange)
			{
				InputMoveBegin moveBegin(_owner, (targetPos - ownerPos).getPerp().getNormalized());
				moveBegin.execute();
			}
			else
			{
				InputMoveBegin moveBegin(_owner, _movingStrategy(_owner));
				moveBegin.execute();
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
	if (isInactive())
		activate();

	duration<double> endTime = 
		system_clock::now().time_since_epoch();

	if (!_attacked)
	{
		if (_owner->getTargetSys()->isTargetPresent())
		{
			Vec2 targetPos = _owner->getTargetSys()->getTarget()->getWorldPosition();
			Vec2 ownerPos = _owner->getWorldPosition();
			Vec2 aim = (targetPos - ownerPos).getNormalized();

			InputBezelBegin bezelBegin(_owner, aim);
			bezelBegin.execute();

			if (_owner->getTargetSys()->isTargetAttackable())
			{
				InputAttackBegin attackBegin(_owner);
				attackBegin.execute();
				_attacked = true;
			}
		}

		_startTime = system_clock::now().time_since_epoch();
	}

	if (kDelayForCompleteAttack < (endTime - _startTime).count())
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


