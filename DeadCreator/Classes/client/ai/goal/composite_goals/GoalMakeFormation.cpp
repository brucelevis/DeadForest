#include "GoalMakeFormation.h"
#include "GoalSeekToPosition.hpp"
#include "HumanBase.hpp"
#include "AbstTargetingSystem.h"
#include "SensoryMemory.h"
#include "InputMoveBegin.hpp"
#include "InputMoveEnd.hpp"


using namespace realtrick::client;
using namespace realtrick;
USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalMakeFormation::GoalMakeFormation(HumanBase* owner)
	:
	GoalCompositeBase(owner)
{
	setGoalType(GoalType::MAKE_FORMATION);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalMakeFormation::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	cocos2d::Vec2 positions[9] =
	{
		cocos2d::Vec2(-1, 1), cocos2d::Vec2(0, 1), cocos2d::Vec2(1, 1),
		cocos2d::Vec2(-1, 0), cocos2d::Vec2(0, 0), cocos2d::Vec2(1, 0),
		cocos2d::Vec2(-1, -1), cocos2d::Vec2(0, -1), cocos2d::Vec2(1, -1),
	};

	int idx = random(0, 8);
	HumanBase* leader = _owner->getTargetSys()->getLeader();

	if (leader == nullptr)
	{
		setGoalStatus(GoalStatus::COMPLETED);
		return;
	}

	Vec2 align = Mat3::pointToWorldSpace(
		positions[idx] * 100,
		leader->getHeading(),
		leader->getHeading().getPerp(),
		leader->getWorldPosition());

	addSubgoal(new GoalSeekToPosition(_owner, align));
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalMakeFormation::process()
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
void GoalMakeFormation::terminate()
{
	InputMoveEnd moveEnd(_owner);
	moveEnd.execute();
}


