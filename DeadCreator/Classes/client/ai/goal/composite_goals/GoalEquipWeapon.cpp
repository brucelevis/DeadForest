#include "GoalEquipWeapon.h"
#include "InventoryData.hpp"
#include "HumanBase.hpp"
#include "SensoryMemory.h"
#include "GoalAvoid.h"
#include <set>

using namespace realtrick::client;
using namespace realtrick;
USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalEquipWeapon::GoalEquipWeapon(HumanBase* owner)
	:
	GoalCompositeBase(owner),
	_weaponType(EntityType::DEFAULT)
{
	setGoalType(GoalType::EQUIP_WEAPON);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalEquipWeapon::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	_startTime = std::chrono::system_clock::now().time_since_epoch();
	_owner->useItem(_weaponType);

	// #bug
	// 무기 장착 시 공격범위가 예상과 다름(샷건인데 0이 나옴)
	//if (_owner->getEquipedWeapon() != nullptr)
	//	cocos2d::log(" weapon range : %d   weapon type : %d %d",
	//		_owner->getEquipedWeapon()->getRange(),
	//		_owner->getEquipedWeapon()->getEntityType(), _weaponType);

	int range = 50;
	if (_weaponType == EntityType::ITEM_AXE)
		range = 65;
	else if (_weaponType == EntityType::ITEM_GLOCK17)
		range = 800;
	else if (_weaponType == EntityType::ITEM_M16A2)
		range = 800;
	else if (_weaponType == EntityType::ITEM_M1897)
		range = 200;

	if (_owner->getEquipedWeapon() != nullptr)
		_owner->getSensoryMemory()->setAttackRange(range - 10);
	else
		_owner->getSensoryMemory()->setAttackRange(range);

	addSubgoal(new GoalAvoid(_owner));
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
GoalStatus GoalEquipWeapon::process()
{
	if (isInactive()) activate();

	processSubgoals();
	std::chrono::duration<double> endTime = std::chrono::system_clock::now().time_since_epoch();

	// Wait for equipping item.
	if ((endTime - _startTime).count() > 1.0)
	{
		setGoalStatus(GoalStatus::COMPLETED);
		return getGoalStatus();
	}

	return getGoalStatus();
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalEquipWeapon::terminate()
{
	removeAllSubgoals();
}


int GoalEquipWeapon::evaluate(HumanBase* const owner)
{
	if (owner->getEquipedWeapon() != nullptr)
	{
		EntityType bulletType = owner->getEquipedWeapon()->getBulletType();
		int amount = owner->getInventoryData()->getItemAmount(bulletType);

		if (owner->getEquipedWeapon()->getReservedBullets() > 0 || amount > 0 ||
			owner->getEquipedWeapon()->getEntityType() == EntityType::ITEM_AXE)
		{
			return 0;
		}
	}

	makeEquipItemWeight();

	// Make best choice
	float weight = 0;
	_weaponType = getBestItem(weight);

	return weight;
}


void GoalEquipWeapon::makeEquipItemWeight()
{
	_weightEquipItem.clear();
	_weightEquipItem.emplace(EntityType::ITEM_AXE, -1.0f);
	_weightEquipItem.emplace(EntityType::ITEM_CONSUMPTION, -1.0f);
	_weightEquipItem.emplace(EntityType::ITEM_GLOCK17, -1.0f);
	_weightEquipItem.emplace(EntityType::ITEM_M16A2, -1.0f);
	_weightEquipItem.emplace(EntityType::ITEM_M1897, -1.0f);
	_weightEquipItem.emplace(EntityType::ITEM_STUFF, -1.0f);

	// Set item weight alive for shooting weapons.
	auto inventory = _owner->getInventoryData();
	const auto& items = inventory->getItemLists();

	std::set<int> itemsFiltered;
	for (const auto& item : items)
	{
		if (item)
			itemsFiltered.emplace(item->getEntityType());
	}

	for (const auto& item : itemsFiltered)
	{
		if (EntityType::BULLET_556MM == item)
		{
			_weightEquipItem[EntityType::ITEM_M16A2] = 1.0f;
		}
		else if (EntityType::BULLET_9MM == item)
		{
			_weightEquipItem[EntityType::ITEM_GLOCK17] = 1.0f;
		}
		else if (EntityType::BULLET_SHELL == item)
		{
			_weightEquipItem[EntityType::ITEM_M1897] = 1.0f;
		}
	}

	for (const auto& item : itemsFiltered)
	{
		if (EntityType::ITEM_AXE == item)
		{
			_weightEquipItem[EntityType::ITEM_AXE] = 100.0f;
		}
		else if (EntityType::ITEM_CONSUMPTION == item)
		{
			_weightEquipItem[EntityType::ITEM_CONSUMPTION] = 0.0f;
		}
		else if (EntityType::ITEM_GLOCK17 == item)
		{
			_weightEquipItem[EntityType::ITEM_GLOCK17] *= 200.0f;
		}
		else if (EntityType::ITEM_M16A2 == item)
		{
			_weightEquipItem[EntityType::ITEM_M16A2] *= 200.0f;
		}
		else if (EntityType::ITEM_M1897 == item)
		{
			_weightEquipItem[EntityType::ITEM_M1897] *= 200.0f;
		}
		else if (EntityType::ITEM_STUFF == item)
		{
			_weightEquipItem[EntityType::ITEM_STUFF] = 0.0f;
		}
	}
}

EntityType GoalEquipWeapon::getBestItem(float& weight) const
{
	EntityType bestItem = EntityType::DEFAULT;
	weight = 0.0f;

	for (auto e : _weightEquipItem)
	{
		if (weight < e.second)
		{
			bestItem = e.first;
			weight = e.second;
		}
		cocos2d::log("equip item   item : %d   weight : %f", e.first, e.second);
	}
	return bestItem;
}
