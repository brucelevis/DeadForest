//
//  GoalFindWeapon.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include "GoalFindWeapon.hpp"
#include "HumanBase.hpp"
#include "Game.hpp"
#include "EntityManager.hpp"
#include "EntityBase.hpp"
#include "GoalMoveToPosition.hpp"
#include "GoalEquipWeapon.h"
#include "SensoryMemory.h"
#include "InventoryData.hpp"
#include "InputCommands.hpp"
#include <set>

using namespace realtrick::client;
using namespace realtrick;


GoalFindWeapon::GoalFindWeapon(HumanBase* owner, float character_bias)
	:
	GoalEvaluatable(owner, character_bias),
	_findWeapon(EntityType::DEFAULT),
	_findWeaponData()
{
    setGoalType(GoalType::FIND_WEAPON);
	_findItemMap.emplace(EntityType::ITEM_AXE, FindItemData());
	_findItemMap.emplace(EntityType::ITEM_CONSUMPTION, FindItemData());
	_findItemMap.emplace(EntityType::ITEM_GLOCK17, FindItemData());
	_findItemMap.emplace(EntityType::ITEM_M16A2, FindItemData());
	_findItemMap.emplace(EntityType::ITEM_M1897, FindItemData());
	_findItemMap.emplace(EntityType::ITEM_STUFF, FindItemData());
	_findItemMap.emplace(EntityType::BULLET_556MM, FindItemData());
	_findItemMap.emplace(EntityType::BULLET_9MM, FindItemData());
	_findItemMap.emplace(EntityType::BULLET_SHELL, FindItemData());
}


GoalFindWeapon::~GoalFindWeapon()
{}

void GoalFindWeapon::activate()
{
    setGoalStatus(GoalStatus::ACTIVE);
	addSubgoal(new GoalMoveToPosition(_owner, _findWeaponData.pos));
}


GoalStatus GoalFindWeapon::process()
{
    if ( isInactive() ) activate();
    
	GoalStatus subGoalStatus = processSubgoals();
	EntityBase* item = _owner->getGame()->getEntityManager()->getEntityFromID(_findWeaponData.id);

	if (item == nullptr)
	{
		return GoalStatus::COMPLETED;
	}
	else
	{
		if (subGoalStatus == GoalStatus::COMPLETED)
		{
			InputMoveBegin moveBegin(
				_owner, 
				(item->getWorldPosition() - _owner->getWorldPosition()).getNormalized());

			moveBegin.execute();
		}
	}
		
    return subGoalStatus;
}


void GoalFindWeapon::terminate()
{
	InputMoveEnd moveEnd(_owner);
	moveEnd.execute();
	removeAllSubgoals();
}

int GoalFindWeapon::evaluate(HumanBase* const owner)
{
	makeFindItemWeight();

	// Make best choice
	float weight;

	auto itemData = getBestItem(weight);
	_findWeapon = itemData.first; 
	_findWeaponData = itemData.second;

	return weight;
}

void GoalFindWeapon::makeFindItemWeight()
{
	_weightFindItem.clear();
	_weightFindItem.emplace(EntityType::ITEM_AXE, 3.0f);
	_weightFindItem.emplace(EntityType::ITEM_CONSUMPTION, 0.0f);
	_weightFindItem.emplace(EntityType::ITEM_GLOCK17, 5.0f);
	_weightFindItem.emplace(EntityType::ITEM_M16A2, 6.0f);
	_weightFindItem.emplace(EntityType::ITEM_M1897, 7.0f);
	_weightFindItem.emplace(EntityType::ITEM_STUFF, 0.0f);
	_weightFindItem.emplace(EntityType::BULLET_556MM, 1.0f);
	_weightFindItem.emplace(EntityType::BULLET_9MM, 1.0f);
	_weightFindItem.emplace(EntityType::BULLET_SHELL, 1.0f);

	auto inventory = _owner->getInventoryData();
	const auto& items = inventory->getItemLists();

	std::set<int> itemsFiltered;
	for (const auto& item : items)
	{
		if (item)
			itemsFiltered.emplace(item->getEntityType());
	}

	// Set find-weight for each weapon with current equipped items
	if (_owner->getEquipedWeapon() != nullptr)
	{
		_weightFindItem[EntityType::ITEM_M16A2] = -1.0f;
		_weightFindItem[EntityType::ITEM_GLOCK17] = -1.0f;
		_weightFindItem[EntityType::ITEM_M1897] = -1.0f;
	}

	// Set find-weight for each weapon with current inventory items
	for (const auto& item : itemsFiltered)
	{
		if (EntityType::BULLET_556MM == item)
		{
			_weightFindItem[EntityType::BULLET_556MM] = -1.0f;
			_weightFindItem[EntityType::ITEM_M16A2] *= 10.0f;
		}
		else if (EntityType::BULLET_9MM == item)
		{
			_weightFindItem[EntityType::BULLET_9MM] = -1.0f;
			_weightFindItem[EntityType::ITEM_GLOCK17] *= 10.0f;
		}
		else if (EntityType::BULLET_SHELL == item)
		{
			_weightFindItem[EntityType::BULLET_SHELL] = -1.0f;
			_weightFindItem[EntityType::ITEM_M1897] *= 10.0f;
		}
		else if (EntityType::ITEM_AXE == item)
		{
			_weightFindItem[EntityType::ITEM_AXE] = -1.0f;
		}
		else if (EntityType::ITEM_CONSUMPTION == item)
		{
			_weightFindItem[EntityType::ITEM_CONSUMPTION] = 0.0f;
		}
		else if (EntityType::ITEM_GLOCK17 == item)
		{
			_weightFindItem[EntityType::ITEM_GLOCK17] = -1.0f;
			_weightFindItem[EntityType::BULLET_9MM] *= 10.0f;
		}
		else if (EntityType::ITEM_M16A2 == item)
		{
			_weightFindItem[EntityType::ITEM_M16A2] = -1.0f;
			_weightFindItem[EntityType::BULLET_556MM] *= 10.0f;
		}
		else if (EntityType::ITEM_M1897 == item)
		{
			_weightFindItem[EntityType::ITEM_M1897] = -1.0f;
			_weightFindItem[EntityType::BULLET_SHELL] *= 10.0f;
		}
		else if (EntityType::ITEM_STUFF == item)
		{
			_weightFindItem[EntityType::ITEM_STUFF] = 0.0f;
		}
	}


	for (auto& e : _weightFindItem)
	{
		EntityType itemType = e.first;
		float minDistance = std::numeric_limits<float>::max();
		cocos2d::Vec2 pos;
		int id = -1;

		const auto& items = _owner->getSensoryMemory()->getSensedItems();

		for (auto i : items)
		{
			if (i->getEntityType() != itemType)
				continue;

			float distance = i->getWorldPosition().distance(_owner->getWorldPosition());
			if (minDistance > distance)
			{
				minDistance = distance;
				pos = i->getWorldPosition();
				id = i->getTag();
			}
		}

		if (minDistance > std::numeric_limits<float>::max() - 50)
		{
			e.second = -1.0f;
		}
		else
		{
			e.second *= 500.0f / minDistance;
			_findItemMap[itemType] = FindItemData(minDistance, pos, id);
		}
	}
}


std::pair<EntityType, FindItemData> GoalFindWeapon::getBestItem(float& weight) const
{
	EntityType bestItem = EntityType::DEFAULT;
	FindItemData itemData;
	weight = 0.0f;

	for (auto e : _weightFindItem)
	{
		auto iter = _findItemMap.find(e.first);
		if (weight < e.second)
		{
			bestItem = e.first;
			weight = e.second;

		
			if (iter != std::end(_findItemMap))
			{
				itemData = iter->second;
				//cocos2d::log("item weight in [GoalFindWeapon]  item : %d   weight : %f    distance : %f    id : %d    pos : (%f, %f)"
				//	, e.first, e.second, itemData.distance, itemData.id, itemData.pos.x, itemData.pos.y);
			}
		}
		
	}

	//cocos2d::log("final item id : %d", itemData.id);
	return std::make_pair(bestItem, itemData);
}



