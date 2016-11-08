#include "GoalRunAway.hpp"
#include "GoalSeekToPosition.hpp"
#include "HumanBase.hpp"
#include "AbstTargetingSystem.hpp"
#include "SensoryMemory.hpp"
#include "InventoryData.hpp"

using namespace realtrick::client;
using namespace realtrick;
USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalRunAway::GoalRunAway(HumanBase* owner, float character_bias)
	:
	GoalEvaluatable(owner, character_bias)
{
	setGoalType(GoalType::RUN_AWAY);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalRunAway::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);
	
	if (!_owner->getTargetSys()->isTargetPresent())
		return;

	cocos2d::Vec2 ownerPos = _owner->getWorldPosition();
	cocos2d::Vec2 avoidMove(ownerPos);

	avoidMove = (_owner->getTargetSys()->getTarget()->getWorldPosition() - ownerPos).getPerp().getNormalized() * 800;
	addSubgoal(new GoalSeekToPosition(_owner, _owner->getWorldPosition() + avoidMove));
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalRunAway::process()
{
	if (isInactive())
		activate();

	setGoalStatus(processSubgoals());
	
	return getGoalStatus();
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalRunAway::terminate()
{
	removeAllSubgoals();
}


int GoalRunAway::evaluate(HumanBase* const owner)
{
	if (!owner->getTargetSys()->isTargetPresent())
		return 0;
	
	auto target = owner->getTargetSys()->getTarget();

	if (!target->getSensoryMemory()->isEntityViewable(owner))
		return 0;

	auto elist = _owner->getSensoryMemory()->getListOfRecentlySensedEntities(false);
	auto alist = _owner->getSensoryMemory()->getListOfRecentlySensedEntities(true);

	return std::max((int)elist.size() - (int)alist.size(), 0) * 10;
}
