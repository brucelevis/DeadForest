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
#include <functional>
using namespace realtrick::client;
using namespace realtrick;

int GoalReadyToFight::evaluate(HumanBase* const owner)
{
	if (owner->getEquipedWeapon() != nullptr)
	{
		if (owner->getEquipedWeapon()->getEntityType() == EntityType::ITEM_AXE)
		{
			int random = cocos2d::random(0, 2);

			return random;
		}

		EntityType bulletType = owner->getEquipedWeapon()->getBulletType();
		if (owner->getInventoryData()->getItemAmount(bulletType) > 0)
			return 0;
		else
			return 10;
	}
	else
	{
		return 10;
	}
}

GoalReadyToFight::GoalReadyToFight(HumanBase* owner) : GoalCompositeBase(owner)
{
    setGoalType(GoalType::READY_TO_FIGHT);
}


GoalReadyToFight::~GoalReadyToFight()
{}


void GoalReadyToFight::activate()
{
    setGoalStatus(GoalStatus::ACTIVE);
    
	makeEquipItemWeight();
	makeFindItemWeight();

	// Make best choice
	EntityType bestEquipItem = getBestItem(_weightEquipItem);
	EntityType bestFindItem = getBestItem(_weightFindItem);

	if(bestEquipItem != EntityType::DEFAULT)
		addSubgoal(new GoalEquipWeapon(_owner, bestEquipItem));
	else
		addSubgoal(new GoalFindWeapon(_owner, bestFindItem));
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
		return getGoalStatus();
	}

    return getGoalStatus();
}


void GoalReadyToFight::terminate()
{
	removeAllSubgoals();
}


void GoalReadyToFight::makeEquipItemWeight()
{
	_weightEquipItem.emplace(EntityType::ITEM_AXE, -1.0f);
	_weightEquipItem.emplace(EntityType::ITEM_CONSUMPTION, -1.0f);
	_weightEquipItem.emplace(EntityType::ITEM_GLOCK17, -1.0f);
	_weightEquipItem.emplace(EntityType::ITEM_M16A2, -1.0f);
	_weightEquipItem.emplace(EntityType::ITEM_M1897, -1.0f);
	_weightEquipItem.emplace(EntityType::ITEM_STUFF, -1.0f);

	// Set item weight alive for shooting weapons.
	auto inventory = _owner->getInventoryData();
	const auto& items = inventory->getItemLists();
	for (const auto& item : items)
	{
		if (item && EntityType::BULLET_556MM == item->getEntityType())
		{
			_weightEquipItem[EntityType::ITEM_M16A2] = 1.0f;
		}
		else if (item && EntityType::BULLET_9MM == item->getEntityType())
		{
			_weightEquipItem[EntityType::ITEM_GLOCK17] = 1.0f;
		}
		else if (item && EntityType::BULLET_SHELL == item->getEntityType())
		{
			_weightEquipItem[EntityType::ITEM_M1897] = 1.0f;
		}
	}

	for (const auto& item : items)
	{
		if (item && EntityType::ITEM_AXE == item->getEntityType())
		{
			_weightEquipItem[EntityType::ITEM_AXE] = 100.0f;
		}
		else if (item && EntityType::ITEM_CONSUMPTION == item->getEntityType())
		{
			_weightEquipItem[EntityType::ITEM_CONSUMPTION] = 0.0f;
		}
		else if (item && EntityType::ITEM_GLOCK17 == item->getEntityType())
		{
			_weightEquipItem[EntityType::ITEM_GLOCK17] *= 200.0f;
		}
		else if (item && EntityType::ITEM_M16A2 == item->getEntityType())
		{
			_weightEquipItem[EntityType::ITEM_M16A2] *= 200.0f;
		}
		else if (item && EntityType::ITEM_M1897 == item->getEntityType())
		{
			_weightEquipItem[EntityType::ITEM_M1897] *= 200.0f;
		}
		else if (item && EntityType::ITEM_STUFF == item->getEntityType())
		{
			_weightEquipItem[EntityType::ITEM_STUFF] = 0.0f;
		}
	}
}


void GoalReadyToFight::makeFindItemWeight()
{
	_weightFindItem.emplace(EntityType::ITEM_AXE, 1.0f);
	_weightFindItem.emplace(EntityType::ITEM_CONSUMPTION, 0.0f);
	_weightFindItem.emplace(EntityType::ITEM_GLOCK17, 5.0f);
	_weightFindItem.emplace(EntityType::ITEM_M16A2, 6.0f);
	_weightFindItem.emplace(EntityType::ITEM_M1897, 7.0f);
	_weightFindItem.emplace(EntityType::ITEM_STUFF, 0.0f);
	_weightFindItem.emplace(EntityType::BULLET_556MM, 2.0f);
	_weightFindItem.emplace(EntityType::BULLET_9MM, 3.0f);
	_weightFindItem.emplace(EntityType::BULLET_SHELL, 4.0f);

	auto inventory = _owner->getInventoryData();

	const auto& items = inventory->getItemLists();
	for (const auto& item : items)
	{
		if (item && EntityType::BULLET_556MM == item->getEntityType())
		{
			_weightFindItem[EntityType::ITEM_M16A2] *= 10.0f;
		}
		else if (item && EntityType::BULLET_9MM == item->getEntityType())
		{
			_weightFindItem[EntityType::ITEM_GLOCK17] *= 10.0f;
		}
		else if (item && EntityType::BULLET_SHELL == item->getEntityType())
		{
			_weightFindItem[EntityType::ITEM_M1897] *= 10.0f;
		}
		else if (item && EntityType::ITEM_AXE == item->getEntityType())
		{
			_weightFindItem[EntityType::ITEM_AXE] = -1.0f;
		}
		else if (item && EntityType::ITEM_CONSUMPTION == item->getEntityType())
		{
			_weightFindItem[EntityType::ITEM_CONSUMPTION] = 0.0f;
		}
		else if (item && EntityType::ITEM_GLOCK17 == item->getEntityType())
		{
			_weightFindItem[EntityType::BULLET_9MM] *= 10.0f;
		}
		else if (item && EntityType::ITEM_M16A2 == item->getEntityType())
		{
			_weightFindItem[EntityType::BULLET_556MM] *= 10.0f;
		}
		else if (item && EntityType::ITEM_M1897 == item->getEntityType())
		{
			_weightFindItem[EntityType::BULLET_SHELL] *= 10.0f;
		}
		else if (item && EntityType::ITEM_STUFF == item->getEntityType())
		{
			_weightFindItem[EntityType::ITEM_STUFF] = 0.0f;
		}
	}


	for (auto& e : _weightFindItem)
	{
		EntityType itemType = e.first;

		auto founded = std::find_if(
			std::begin(_owner->getSensoryMemory()->getSensedItems()),
			std::end(_owner->getSensoryMemory()->getSensedItems()),
			[itemType](ItemBase* item) { return item->getEntityType() == itemType; }
		);

		// Don't give weight to non-visible items.
		if (founded == std::end(_owner->getSensoryMemory()->getSensedItems()))
		{
			e.second = -1.0f;
		}
	}
}




EntityType GoalReadyToFight::getBestItem(const std::map<EntityType, float>& weightMap) const
{
	EntityType bestItem = EntityType::DEFAULT;
	float bestItemWeight = 0.0f;

	for (auto e : weightMap)
	{
		if (bestItemWeight < e.second)
		{
			bestItem = e.first;
			bestItemWeight = e.second;
		}
	}
	return bestItem;
}

