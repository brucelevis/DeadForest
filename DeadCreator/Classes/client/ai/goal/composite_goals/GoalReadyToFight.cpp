//
//  GoalReadyToFight.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalReadyToFight.hpp"
#include "Goals.hpp"
#include "HumanBase.hpp"
#include "InventoryData.hpp"
using namespace realtrick::client;


GoalReadyToFight::GoalReadyToFight(HumanBase* owner) : GoalCompositeBase(owner)
{
    setGoalType(GoalType::READY_TO_FIGHT);
}


GoalReadyToFight::~GoalReadyToFight()
{
    CC_SAFE_DELETE(_goalFindWeapon);
}


void GoalReadyToFight::activate()
{
	cocos2d::log("GoalReadyToFight::activate()");
    setGoalStatus(GoalStatus::ACTIVE);
    
	// use item
	auto inventory = _owner->getInventoryData();
	const auto& items = inventory->getItemLists();
	for (const auto& item : items)
	{
		if ( item && EntityType::ITEM_AXE == item->getEntityType() )
		{
			item->use();
			break;
		}
	}
}


GoalStatus GoalReadyToFight::process()
{
	cocos2d::log("GoalReadyToFight::process()");
	if (isInactive())
		activate();

	setGoalStatus(GoalStatus::COMPLETED);
    return getGoalStatus();
}


void GoalReadyToFight::terminate()
{
}









