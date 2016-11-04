#include "GoalMakeFormation.hpp"
#include "GoalSeekToPosition.hpp"
#include "HumanBase.hpp"
#include "AbstTargetingSystem.hpp"
#include "SensoryMemory.hpp"
#include "InputMoveBegin.hpp"
#include "InputMoveEnd.hpp"
#include "InputBezelBegin.hpp"
#include "InputBezelEnd.hpp"

#include "GoalNetwork.hpp"


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
	
	HumanBase* leader = _owner->getTargetSys()->getLeader();

	if (leader == nullptr)
	{
		setGoalStatus(GoalStatus::COMPLETED);
		return;
	}

	int idx = leader->getTargetSys()->queryFollowerIndex(_owner);

	// Has no leader
	if (idx < 0)
		return;

	Vec2 align = Mat3::pointToWorldSpace(
		GoalNetwork::kFormations[idx] * GoalNetwork::kFormationDistance,
		-leader->getHeading().getPerp(),
		-leader->getHeading().getPerp().getPerp(),
		leader->getWorldPosition());

	if (align.distance(_owner->getWorldPosition()) < _owner->getBoundingRadius() * 2)
	{
		InputMoveEnd moveEnd(_owner);
		moveEnd.execute();
	}
	else
	{
		InputMoveBegin moveBegin(_owner, (align - _owner->getWorldPosition()).getNormalized());
		moveBegin.execute();
	}
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalMakeFormation::process()
{
	// If status is INACTIVE, call activate()
	if (isInactive())
		activate();

	return GoalStatus::COMPLETED;
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalMakeFormation::terminate()
{}


