//
//  GoalReadyToFight.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalReadyToFight.hpp"
#include "GoalEquipWeapon.h"
#include "Goals.hpp"
#include "HumanBase.hpp"
#include "InventoryData.hpp"
#include "SensoryMemory.h"
#include <map>
using namespace realtrick::client;


GoalReadyToFight::GoalReadyToFight(HumanBase* owner) : GoalCompositeBase(owner)
{
    setGoalType(GoalType::READY_TO_FIGHT);
}


GoalReadyToFight::~GoalReadyToFight()
{}


void GoalReadyToFight::activate()
{
	cocos2d::log("GoalReadyToFight::activate()");
    setGoalStatus(GoalStatus::ACTIVE);
    
	// Make best choice
	EntityType bestItem = EntityType::DEFAULT;

	auto inventory = _owner->getInventoryData();
	const auto& items = inventory->getItemLists();
	for (const auto& item : items)
	{
		if ( item && EntityType::ITEM_AXE == item->getEntityType() )
		{
			bestItem = EntityType::ITEM_AXE;
			return;
		}
	}

	if(bestItem != EntityType::DEFAULT)
		addSubgoal(new GoalEquipWeapon(_owner, bestItem));
	else
		addSubgoal(new GoalFindWeapon(_owner));
	
	//_owner->useItem(EntityType::ITEM_AXE);
	//if(_owner->getEquipedWeapon() != nullptr)
	//	_owner->getSensoryMemory()->setAttackRange(_owner->getEquipedWeapon()->getRange() - 10);
}


GoalStatus GoalReadyToFight::process()
{
	if (isInactive())
		activate();

	//process the subgoals
	_goalStatus = processSubgoals();

	//if any of the subgoals have FAILED then this goal re-plans
	if (_goalStatus == GoalStatus::FAILED)
	{
		cocos2d::log("failed in GoalReadyToFight::process()");
		setGoalStatus(GoalStatus::INACTIVE);
	}

    return getGoalStatus();
}


void GoalReadyToFight::terminate()
{
	cocos2d::log("GoalReadyToFight::terminate()");
	removeAllSubgoals();
}









