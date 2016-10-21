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
USING_NS_CC;


GoalFindWeapon::GoalFindWeapon(HumanBase* owner, float character_bias)
	:
	GoalEvaluatable(owner, character_bias),
	_findWeaponData()
{
    setGoalType(GoalType::FIND_WEAPON);
	_findItemMap.resize(EntityType::MAX, FindItemData());
	_weightFindItem.resize(EntityType::MAX, 0.0f);
}

void GoalFindWeapon::activate()
{
    setGoalStatus(GoalStatus::ACTIVE);
	addSubgoal(new GoalMoveToPosition(_owner, _findWeaponData.pos));
}


GoalStatus GoalFindWeapon::process()
{
	if (isInactive())
		activate();
    
	GoalStatus subGoalStatus = processSubgoals();
	EntityBase* item = 
		_owner->getGame()->getEntityManager()->getEntityFromID(_findWeaponData.id);

	if (item == nullptr)
		return GoalStatus::COMPLETED;
		
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
	_findWeaponData = itemData.second;

	return weight;
}

void GoalFindWeapon::makeFindItemWeight()
{
	_weightFindItem[ITEM_AXE] = 3.0f;
	_weightFindItem[ITEM_GLOCK17] = 5.0f;
	_weightFindItem[ITEM_M16A2] = 6.0f;
	_weightFindItem[ITEM_M1897] = 7.0f;
	_weightFindItem[BULLET_556MM] = 1.0f;
	_weightFindItem[BULLET_9MM] = 1.0f;
	_weightFindItem[BULLET_SHELL] = 1.0f;

	auto inventory = _owner->getInventoryData();
	const auto& items = inventory->getItemLists();

	// Setting bullet..
	std::array<int, EntityType::MAX> itemsFiltered{ false };

	for (const auto& item : items)
		if (item)
			itemsFiltered[item->getEntityType()] = true;

	// Set find-weight for each weapon with current equipped items
	if (_owner->getEquipedWeapon() != nullptr)
	{
		_weightFindItem[EntityType::ITEM_M16A2] = -1.0f;
		_weightFindItem[EntityType::ITEM_GLOCK17] = -1.0f;
		_weightFindItem[EntityType::ITEM_M1897] = -1.0f;
	}

	// Set find-weight for each weapon with current inventory items
	for (size_t i = 0; i < itemsFiltered.size(); ++i)
	{
		if (!itemsFiltered[i])
			continue;

		EntityType item = (EntityType)i;

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
	}

	// Set not sensed items with invalid value,
	// and set sensed items with the distance.
	for (size_t i = 0; i < _weightFindItem.size(); ++i)
	{
		EntityType item = (EntityType)i;

		float minDistance = std::numeric_limits<float>::max();
		Vec2 pos;
		int id = cocos2d::Node::INVALID_TAG;

		const auto& items = _owner->getSensoryMemory()->getSensedItems();

		for (auto i : items)
		{
			if (i->getEntityType() != item)
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
			_weightFindItem[item] = -1.0f;
		}
		else
		{
			_weightFindItem[item] *= 500.0f / minDistance;
			_findItemMap[item] = FindItemData(minDistance, pos, id);
		}
	}
}


std::pair<EntityType, FindItemData> GoalFindWeapon::getBestItem(float& weight) const
{
	EntityType bestItem = EntityType::DEFAULT;
	FindItemData itemData;
	weight = 0.0f;

	for (size_t i = 0; i < _weightFindItem.size(); ++i)
	{
		EntityType item = (EntityType)i;

		if (weight < _weightFindItem[item])
		{
			bestItem = item;
			weight = _weightFindItem[item];

			if (_findItemMap[item].id != cocos2d::Node::INVALID_TAG)
			{
				itemData = _findItemMap[item];
				//log("item weight in [GoalFindWeapon]  item : %d   weight : %f    distance : %f    id : %d    pos : (%f, %f)"
					//, e.first, e.second, itemData.distance, itemData.id, itemData.pos.x, itemData.pos.y);
			}
		}
	}

	//log("final item id : %d", itemData.id);
	return std::make_pair(bestItem, itemData);
}



