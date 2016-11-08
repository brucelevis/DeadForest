#include "GoalAmbush.hpp"
#include "GoalSeekToPosition.hpp"
#include "GoalAttackTarget.hpp"
#include "HumanBase.hpp"
#include "AbstTargetingSystem.hpp"
#include "SensoryMemory.hpp"
#include "InventoryData.hpp"

using namespace realtrick::client;
using namespace realtrick;
USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalAmbush::GoalAmbush(HumanBase* owner, float character_bias)
	:
	GoalEvaluatable(owner, character_bias)
{
	setGoalType(GoalType::AMBUSH);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalAmbush::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	if (!_owner->getTargetSys()->isTargetPresent())
		return;
	
	addSubgoal(new GoalAttackTarget(_owner));
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalAmbush::process()
{
	if (isInactive())
		activate();

	setGoalStatus(processSubgoals());
	
	return getGoalStatus();
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalAmbush::terminate()
{
	removeAllSubgoals();
}


int GoalAmbush::evaluate(HumanBase* const owner)
{
	return 1;
}
