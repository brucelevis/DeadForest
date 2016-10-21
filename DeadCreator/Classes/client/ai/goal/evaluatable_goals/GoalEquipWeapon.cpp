#include "GoalEquipWeapon.h"
#include "InventoryData.hpp"
#include "HumanBase.hpp"
#include "SensoryMemory.h"
#include "GoalAvoid.h"
#include <set>

namespace
{
	const float kEquippingTime = 1.0f;
	const int kNumOfRangeWeapon = 4;

	const float kWeightAXE = 100.0f;
	const float kWeightGLOCK17 = 100.0f;
	const float kWeightM16A2 = 100.0f;
	const float kWeightM1897 = 100.0f;
}

using namespace realtrick::client;
using namespace realtrick;
using namespace std::chrono;
USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalEquipWeapon::GoalEquipWeapon(HumanBase* owner, float character_bias)
	:
	GoalEvaluatable(owner, character_bias),
	_weaponType(EntityType::DEFAULT),
	_weightEquipItem{}
{
	setGoalType(GoalType::EQUIP_WEAPON);
}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalEquipWeapon::activate()
{
	setGoalStatus(GoalStatus::ACTIVE);

	_startTime = system_clock::now().time_since_epoch();

	_owner->useItem(_weaponType);

	// #bug
	// 무기 장착 시 공격범위가 예상과 다름(샷건인데 0이 나옴)
	//if (_owner->getEquipedWeapon() != nullptr)
	//	cocos2d::log(" weapon range : %d   weapon type : %d %d",
	//		_owner->getEquipedWeapon()->getRange(),
	//		_owner->getEquipedWeapon()->getEntityType(), _weaponType);

	int range = 55;
	if (_weaponType == ITEM_AXE)
		range = 60;
	else if (_weaponType == ITEM_GLOCK17)
		range = 750;
	else if (_weaponType == ITEM_M16A2)
		range = 750;
	else if (_weaponType == ITEM_M1897)
		range = 160;

	if (_owner->getEquipedWeapon() != nullptr)
		_owner->getSensoryMemory()->setAttackRange(range);
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
	duration<double> endTime = 
		system_clock::now().time_since_epoch();

	// Wait for equipping item.
	if ((endTime - _startTime).count() > kEquippingTime)
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


void GoalEquipWeapon::makeEquipItemWeight()
{
	std::array<int, kNumOfRangeWeapon> bulletsByWeapon{};

	auto inventory = _owner->getInventoryData();
	const auto& items = inventory->getItemLists();

	// Setting bullet..
	std::array<int, EntityType::MAX> itemsFiltered{false};

	for (const auto& item : items)
	{
		if (item)
		{
			if (item->getEntityType() == BULLET_556MM ||
				item->getEntityType() == BULLET_SHELL ||
				item->getEntityType() == BULLET_9MM)
				continue;

			if (item->getEntityType() == ITEM_M16A2 ||
				item->getEntityType() == ITEM_M1897 ||
				item->getEntityType() == ITEM_GLOCK17)
			{
				WeaponBase* weapon = static_cast<WeaponBase*>(item);
				bulletsByWeapon[item->getEntityType()] += weapon->getNumOfLeftRounds();
			}
			itemsFiltered[item->getEntityType()] = true;
		}
	}

	bulletsByWeapon[EntityType::ITEM_GLOCK17] += inventory->getItemAmount(BULLET_9MM);
	bulletsByWeapon[EntityType::ITEM_M16A2] += inventory->getItemAmount(BULLET_556MM);
	bulletsByWeapon[EntityType::ITEM_M1897] += inventory->getItemAmount(BULLET_SHELL);


	//cocos2d::log("BULLET_9MM : %d   BULLET_556MM : %d   BULLET_SHELL : %d",
	//	inventory->getItemAmount(BULLET_9MM),
	//	inventory->getItemAmount(BULLET_556MM),
	//	inventory->getItemAmount(BULLET_SHELL));
		
	// Weight setting with weapons.
	for (size_t i = 0; i < EntityType::MAX; i++)
	{
		EntityType item = (EntityType)i;
		if (!itemsFiltered[i])
			continue;

		if (EntityType::ITEM_AXE == item)
		{
			_weightEquipItem[EntityType::ITEM_AXE] = kWeightAXE;
		}
		else if (EntityType::ITEM_GLOCK17 == item)
		{
			_weightEquipItem[EntityType::ITEM_GLOCK17] = 
				kWeightGLOCK17 * (float)bulletsByWeapon[EntityType::ITEM_GLOCK17] / 21.0f;
		}
		else if (EntityType::ITEM_M16A2 == item)
		{
			_weightEquipItem[EntityType::ITEM_M16A2] =
				kWeightM16A2 * (float)bulletsByWeapon[EntityType::ITEM_M16A2] / 90.0f;
		}
		else if (EntityType::ITEM_M1897 == item)
		{
			_weightEquipItem[EntityType::ITEM_M1897] = 
				kWeightM1897 * (float)bulletsByWeapon[EntityType::ITEM_M1897] / 30.0f;
		}
	}

	
}

EntityType GoalEquipWeapon::getBestItem(float& weight) const
{
	EntityType bestItem = EntityType::DEFAULT;
	weight = 0.0f;

	for (size_t i = 0; i < _weightEquipItem.size(); ++i)
	{
		if (weight < _weightEquipItem[i])
		{
			bestItem = (EntityType)i;
			weight = _weightEquipItem[i];
		}
		//cocos2d::log("item weight in [GoalEquipWeapon]   item : %d   weight : %f", i, _weightEquipItem[i]);
	}

	return bestItem;
}


int GoalEquipWeapon::evaluate(HumanBase* const owner)
{
	EntityType pastEquipped = EntityType::DEFAULT;

	if (owner->getEquipedWeapon() != nullptr)
	{
		pastEquipped = (EntityType)owner->getEquipedWeapon()->getEntityType();

		EntityType bulletType = owner->getEquipedWeapon()->getBulletType();
		int amount = owner->getInventoryData()->getItemAmount(bulletType);

		if (owner->getEquipedWeapon()->getNumOfLeftRounds() > 0 || amount > 0 ||
			owner->getEquipedWeapon()->getEntityType() == EntityType::ITEM_AXE)
		{
			if (!owner->getSensoryMemory()->queryUsableRangeWeaponExists(pastEquipped))
				return 0;
		}
	}

	makeEquipItemWeight();

	// Make best choice
	float weight = 0;
	_weaponType = getBestItem(weight);

	if (pastEquipped == _weaponType)
		return 0;

	return weight;
}
