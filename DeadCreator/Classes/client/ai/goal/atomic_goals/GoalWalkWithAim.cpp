#include "GoalWalkWithAim.h"
#include "AbstTargetingSystem.h"
#include "SensoryMemory.h"
#include "HumanBase.hpp"
#include "InventoryData.hpp"
#include "InputBezelBegin.hpp"
#include "InputBezelEnd.hpp"
#include "InputMoveBegin.hpp"
#include "InputMoveEnd.hpp"
#include "StateMachine.hpp"

using namespace realtrick::client;
USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalWalkWithAim::GoalWalkWithAim(
	HumanBase* owner,
	const cocos2d::Vec2& destination,
	const cocos2d::Vec2& aimHeading,
	float walkTime)
	:
	GoalBase(owner),
	_destination(destination),
	_aimHeading(aimHeading),
	_walkTime(walkTime)
{
	setGoalType(GoalType::WALK_WITH_AIM);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalWalkWithAim::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	_startTime = std::chrono::system_clock::now().time_since_epoch();

	InputMoveBegin moveBegin(
		_owner, 
		(_destination - _owner->getWorldPosition()).getNormalized());

	moveBegin.execute();

	InputBezelBegin bezelBegin(_owner, _aimHeading);
	bezelBegin.execute();
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalWalkWithAim::process()
{
	if (isInactive())
		activate();

	std::chrono::duration<double> endTime = 
		std::chrono::system_clock::now().time_since_epoch();

	if (_walkTime < (endTime - _startTime).count())
		setGoalStatus(GoalStatus::COMPLETED);
	
	return getGoalStatus();
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalWalkWithAim::terminate()
{
	InputMoveEnd moveEnd(_owner);
	moveEnd.execute();

	InputBezelEnd bezelEnd(_owner);
	bezelEnd.execute();

	setGoalStatus(GoalStatus::COMPLETED);
}


