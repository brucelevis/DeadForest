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
#include <set>

using namespace realtrick::client;
using namespace realtrick;


GoalFindWeapon::GoalFindWeapon(HumanBase* owner) 
	:
	GoalCompositeBase(owner),
	_findWeapon(EntityType::DEFAULT)
{
    setGoalType(GoalType::FIND_WEAPON);
}


GoalFindWeapon::~GoalFindWeapon()
{}

void GoalFindWeapon::activate()
{
    setGoalStatus(GoalStatus::ACTIVE);

	const float bigf = std::numeric_limits<float>::max();
	float dist = bigf;
	cocos2d::Vec2 desti;

	cocos2d::log("GoalFindWeapon find : %d", _findWeapon);

	for (auto e : _owner->getGame()->getEntityManager()->getEntities())
	{
		if (e.second->getEntityType() == _findWeapon)
		{
			cocos2d::Vec2 itemPos = e.second->getWorldPosition();
			float distance = itemPos.distance(_owner->getWorldPosition());

			if (distance > _owner->getSensoryMemory()->getViewRange())
				continue;

			if (dist > distance)
			{
				dist = distance;
				desti = itemPos;
			}
		}
	}

	if (dist < bigf - 1)
	{
		addSubgoal(new GoalMoveToPosition(_owner, desti));
	}
	else
	{
		cocos2d::log("FAILED in GoalFindWeapon");
		setGoalStatus(GoalStatus::FAILED);
	}
}


GoalStatus GoalFindWeapon::process()
{
    if ( isInactive() ) activate();
    
    auto subGoalStatus = processSubgoals();
    return subGoalStatus;
}


void GoalFindWeapon::terminate()
{
	removeAllSubgoals();
}

int GoalFindWeapon::evaluate(HumanBase* const owner)
{
	makeFindItemWeight();

	// Make best choice
	float weight;
	_findWeapon = getBestItem(weight);
	
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
		cocos2d::log("has weapon!! in makeFindItemWeight()");
		//_weightFindItem[EntityType::ITEM_AXE] = -1.0f;
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
		else
		{
			cocos2d::log("found item : %d", e.first);
		}
	}
}


EntityType GoalFindWeapon::getBestItem(float& weight) const
{
	EntityType bestItem = EntityType::DEFAULT;
	weight = 0.0f;

	for (auto e : _weightFindItem)
	{
		if (weight < e.second)
		{
			bestItem = e.first;
			weight = e.second;
		}
		cocos2d::log("item weight in [GoalFindWeapon]  item : %d   weight : %f", e.first, e.second);
	}
	return bestItem;
}



