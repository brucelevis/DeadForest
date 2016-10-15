#include "SensoryMemory.h"
#include "Game.hpp"
#include "EntityBase.hpp"
#include "CellSpacePartition.hpp"
#include "InventoryData.hpp"
#include "AbstTargetingSystem.h"
#include <sstream>
#include <algorithm>

namespace
{
	float kDefaultViewRange = 600.0f;
	float kDefaultAttackRange = 60.0f;
}

USING_NS_CC;
using namespace realtrick::client;

std::stringstream & realtrick::client::operator<<(
	std::stringstream &ss,
	const MemoryRecord& m)
{
	ss << m.attackable << std::endl
		<< m.viewable << std::endl
		<< m.timeBecameVisible.count() << std::endl
		<< m.timeLastSensed.count() << std::endl
		<< m.timeLastVisible.count() << std::endl
		<< m.recentDamage << std::endl
		<< m.lastSensedPos.x << ", "
		<< m.lastSensedPos.y << std::endl;
	return ss;
}

//------------------------------- ctor ----------------------------------------
//-----------------------------------------------------------------------------
SensoryMemory::SensoryMemory(
	HumanBase* const owner,
	double memory_span)
	:
	_owner(owner),
	_memorySpan(memory_span)
{
	_viewRange = kDefaultViewRange;
	_attackRange = kDefaultAttackRange;
}

SensoryMemory::~SensoryMemory()
{}

//--------------------- makeNewRecordIfNotAlreadyPresent ----------------------

void SensoryMemory::makeNewRecordIfNotAlreadyPresent(HumanBase* const opponent)
{
	//else check to see if this Opponent already exists in the memory. If it doesn't,
	//create a new record
	if (_memory.find(opponent) == _memory.end())
	{
		_memory[opponent] = MemoryRecord();
	}
}

//------------------------ removeBotFromMemory --------------------------------
//
//  this removes a bot's record from memory
//-----------------------------------------------------------------------------
void SensoryMemory::removeBotFromMemory(HumanBase* const bot)
{
	MemoryMap::iterator record = _memory.find(bot);

	if (record != _memory.end())
	{
		_memory.erase(record);
	}
}


void SensoryMemory::removeItemFromMemory(ItemBase* const item)
{
	auto record = std::find(std::begin(_sensedItems), std::end(_sensedItems), item);

	if (record != std::end(_sensedItems))
	{
		_sensedItems.erase(record);
	}
}


//----------------------------- updateVision ----------------------------------
//
//  this method iterates through all the bots in the game world to test if
//  they are in the field of view. Each bot's memory record is updated
//  accordingly
//-----------------------------------------------------------------------------
void SensoryMemory::updateVision()
{
	const auto& elist = _owner->getGame()->getNeighborsOnMove(_owner->getWorldPosition(), _viewRange);

	for (auto iter = std::begin(elist); iter != std::end(elist); iter++)
	{
		if (_owner->getTag() != (*iter)->getTag())
		{
			if (isMasked((*iter)->getFamilyMask(), FamilyMask::HUMAN_BASE))
			{
				HumanBase* human = static_cast<HumanBase*>(*iter);

				//make sure it is part of the memory map
				makeNewRecordIfNotAlreadyPresent(human);

				//get a reference to this bot's data
				MemoryRecord& info = _memory[human];

				cocos2d::Vec2 ownerPos = _owner->getWorldPosition();
				cocos2d::Vec2 humanPos = human->getWorldPosition();

				if (ownerPos.getDistance(humanPos) < _viewRange)
				{
					info.isLosOkay = _owner->getGame()->isLOSOkay(ownerPos, humanPos);

					if (info.isLosOkay)
					{
						info.timeLastSensed =
							std::chrono::system_clock::now().time_since_epoch();

						info.timeLastVisible =
							std::chrono::system_clock::now().time_since_epoch();

						info.lastSensedPos = humanPos;

						if (ownerPos.getDistance(humanPos) < _attackRange &&
							info.isLosOkay)
							info.attackable = true;
						else
							info.attackable = false;

						if (info.viewable == false &&
							info.isLosOkay)
						{
							info.viewable = true;
							info.timeBecameVisible = info.timeLastSensed;
						}
					}
				}
				else
				{
					info.attackable = false;
					info.viewable = false;
				}
			}
			else if (isMasked((*iter)->getFamilyMask(), FamilyMask::ITEM_BASE))
			{
				ItemBase* item = static_cast<ItemBase*>(*iter);
				if (_owner->getWorldPosition().getDistance(item->getWorldPosition()) < _viewRange)
				{
					auto finded = std::find(std::begin(_sensedItems), std::end(_sensedItems), item);

					if (finded == std::end(_sensedItems))
						_sensedItems.push_back(item);
				}
			}
		}
	}
}


//------------------------ getListOfRecentlySensedOpponents -------------------
//
//  returns a list of the bots that have been sensed recently
//-----------------------------------------------------------------------------
std::list<HumanBase*>
SensoryMemory::getListOfRecentlySensedEntities(bool ally) const
{
	//this will store all the opponents the bot can remember
	std::list<HumanBase*> opponents;

	std::chrono::duration<double> current_time = std::chrono::system_clock::now().time_since_epoch();

	for(auto& rec : _memory)
	{
		if ((rec.first)->isAlive() && (rec.first != _owner))
		{
			if (!(ally ^ _owner->getGame()->isAllyState(
				_owner->getPlayerType(),
				(rec.first)->getPlayerType())))
			{
				//if this bot has been updated in the memory recently, add to list
				if ((current_time - rec.second.timeLastSensed) <= _memorySpan)
				{
					opponents.push_back(rec.first);
				}
			}
		}
	}

	return opponents;
}

const std::vector<ItemBase*>& SensoryMemory::getSensedItems() const
{
	return _sensedItems;
}

//----------------------------- isOpponentAttackable --------------------------------
//
//  returns true if the bot given as a parameter can be shot (ie. its not
//  obscured by walls)
//-----------------------------------------------------------------------------
bool SensoryMemory::isOpponentAttackable(HumanBase* const opponent)const
{
	MemoryMap::const_iterator it = _memory.find(opponent);

	if (it != _memory.end())
	{
		return it->second.attackable; 
	}

	return false;
}

//----------------------------- isOpponentWithinFOV --------------------------------
//
//  returns true if the bot given as a parameter is within FOV
//-----------------------------------------------------------------------------
bool SensoryMemory::isOpponentWithinFOV(HumanBase* const opponent) const
{
	MemoryMap::const_iterator it = _memory.find(opponent);

	if (it != _memory.end())
	{
		return it->second.viewable;
	}

	return false;
}

//---------------------------- getLastRecordedPositionOfOpponent -------------------
//
//  returns the last recorded position of the bot
//-----------------------------------------------------------------------------
Vec2 SensoryMemory::getLastRecordedPositionOfOpponent(HumanBase* const opponent) const
{
	MemoryMap::const_iterator it = _memory.find(opponent);

	if (it != _memory.end())
	{
		return it->second.lastSensedPos;
	}

	throw std::runtime_error("< SensoryMemory::getLastRecordedPositionOfOpponent>: Attempting to get position of unrecorded bot");
}

//----------------------------- getTimeOpponentHasBeenVisible ----------------------
//
//  returns the amount of time the given bot has been visible
//-----------------------------------------------------------------------------
std::chrono::duration<double> SensoryMemory::getTimeOpponentHasBeenVisible(HumanBase* const opponent) const
{
	MemoryMap::const_iterator it = _memory.find(opponent);

	if (it != _memory.end() && it->second.viewable)
	{
		return std::chrono::system_clock::now().time_since_epoch() - it->second.timeBecameVisible;
	}

	return std::chrono::duration<double>(0);
}

int SensoryMemory::getDamage(HumanBase* const opponent)const
{
	MemoryMap::const_iterator it = _memory.find(opponent);

	if (it != _memory.end())
	{
		return it->second.recentDamage;
	}

	return 0;
}

//-------------------- getTimeOpponentHasBeenOutOfView ------------------------
//
//  returns the amount of time the given opponent has remained out of view
//  returns a high value if opponent has never been seen or not present
//-----------------------------------------------------------------------------
std::chrono::duration<double> SensoryMemory::getTimeOpponentHasBeenOutOfView(HumanBase* const opponent)const
{
	MemoryMap::const_iterator it = _memory.find(opponent);

	if (it != _memory.end())
	{
		return std::chrono::system_clock::now().time_since_epoch() - it->second.timeLastVisible;
	}

	return std::chrono::duration<double>(std::numeric_limits<double>::max());
}

//------------------------ getTimeSinceLastSensed ----------------------
//
//  returns the amount of time the given bot has been visible
//-----------------------------------------------------------------------------
std::chrono::duration<double> SensoryMemory::getTimeSinceLastSensed(HumanBase* const opponent)const
{
	MemoryMap::const_iterator it = _memory.find(opponent);

	if (it != _memory.end() && it->second.viewable)
	{
		return std::chrono::system_clock::now().time_since_epoch() - it->second.timeLastSensed;
	}

	return std::chrono::duration<double>(0);
}

bool SensoryMemory::isUnderAttack() const
{
	for (auto& rec : _memory)
	{
		//if this bot has hit us, return true
		if (rec.second.recentDamage > 0)
			return true;
	}
	return false;
}

cocos2d::Vec2 SensoryMemory::avoidingEnemiesVector(cocos2d::Vec2& pos, cocos2d::Vec2& heading)
{
	cocos2d::Vec2 avoidMove(heading * std::numeric_limits<float>::min());
	const auto& enemies = getListOfRecentlySensedEntities(false);

	for (auto e : enemies)
	{
		cocos2d::Vec2 ePos = e->getWorldPosition();
		cocos2d::Vec2 toTarget = ePos - pos;
		avoidMove += -toTarget / toTarget.lengthSquared();
	}
	return avoidMove.getNormalized();
}


cocos2d::Vec2 SensoryMemory::avoidingEnemiesVector(HumanBase* const owner)
{
	cocos2d::Vec2 avoidMove(owner->getHeading() * std::numeric_limits<float>::min());
	const auto& enemies = getListOfRecentlySensedEntities(false);

	for (auto e : enemies)
	{
		if (!(e->getTargetSys()->getTarget() == owner))
			continue;

		cocos2d::Vec2 ePos = e->getWorldPosition();
		cocos2d::Vec2 toTarget = ePos - owner->getWorldPosition();
		avoidMove += -toTarget / toTarget.lengthSquared();
	}
	return avoidMove.getNormalized();
}



std::vector<ItemBase*> SensoryMemory::queryMeleeWeapon()
{
	std::vector<ItemBase*> weapons;

	auto inventory = _owner->getInventoryData();
	const auto& items = inventory->getItemLists();

	for (const auto& item : items)
	{
		if(item != nullptr)
			if (item->getEntityType() == EntityType::ITEM_AXE)
				weapons.emplace_back(item);
	}
	return weapons;
}

std::vector<ItemBase*> SensoryMemory::queryUsableRangeWeapon()
{
	auto inventory = _owner->getInventoryData();
	const auto& items = inventory->getItemLists();

	std::vector<ItemBase*> weapons;
	std::vector<ItemBase*> bullets = queryBullets();

	for (const auto& b : bullets)
	{
		if (b == nullptr)
			continue;

		EntityType findType = EntityType::DEFAULT;
		if (b->getEntityType() == EntityType::BULLET_556MM)
		{
			findType = EntityType::ITEM_M16A2;
		}
		else if (b->getEntityType() == EntityType::BULLET_9MM)
		{
			findType = EntityType::ITEM_GLOCK17;
		}
		else if (b->getEntityType() == EntityType::BULLET_SHELL)
		{
			findType = EntityType::ITEM_M1897;
		}
		auto iter = std::find_if(std::begin(items), std::end(items),
			[findType](ItemBase* i) { return (i != nullptr) && i->getEntityType() == findType; });

		if(iter != std::end(items))
			weapons.push_back(*iter);
	}
	return weapons;
}

std::vector<ItemBase*> SensoryMemory::queryBullets()
{
	std::vector<ItemBase*> bullets;

	auto inventory = _owner->getInventoryData();
	const auto& items = inventory->getItemLists();

	for (const auto& item : items)
	{
		if (item != nullptr)
		{
			if (item->getEntityType() == EntityType::BULLET_556MM || 
				item->getEntityType() == EntityType::BULLET_9MM ||
				item->getEntityType() == EntityType::BULLET_SHELL)
				bullets.emplace_back(item);
		}
	}
	return bullets;
}


bool SensoryMemory::isReadyToFight() const
{
	// no weapon
	if (_owner->getEquipedWeapon() == nullptr)
		return false;

	// equipped axe
	if (_owner->getEquipedWeapon()->getEntityType() == EntityType::ITEM_AXE)
	{
		return true;
	}
	
	// equipped range weapon and has bullet
	if (_owner->getEquipedWeapon()->getNumOfLeftRounds() > 0)
		return true;

	// equipped range weapon and has no bullet ready, but has it in inventory
	auto inventory = _owner->getInventoryData();
	const auto& items = inventory->getItemLists();

	for (const auto& item : items)
	{
		if (item != nullptr)
			if (item->getEntityType() == _owner->getEquipedWeapon()->getBulletType())
				return true;
	}
	return false;
}