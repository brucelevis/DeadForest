#include "SensoryMemory.h"
#include "Game.hpp"
#include "EntityBase.hpp"
#include "CellSpacePartition.hpp"
#include <sstream>

USING_NS_CC;
using namespace realtrick::client;

std::stringstream & realtrick::client::operator<<(std::stringstream &ss, const MemoryRecord& m)
{
	ss << m.attackable << std::endl
		<< m.within_view << std::endl
		<< m.time_became_visible.count() << std::endl
		<< m.time_last_sensed.count() << std::endl
		<< m.time_last_visible.count() << std::endl
		<< m.recent_damage << std::endl
		<< m.last_sensed_position.x << ", "
		<< m.last_sensed_position.y << std::endl;
	return ss;
}

//------------------------------- ctor ----------------------------------------
//-----------------------------------------------------------------------------
SensoryMemory::SensoryMemory(
	HumanBase* const owner,
	double memory_span)
	:
	_owner(owner),
	_memory_span(memory_span)
{
	_viewRange = 200;
	_attackRange = 60;
}

SensoryMemory::~SensoryMemory()
{}

//--------------------- makeNewRecordIfNotAlreadyPresent ----------------------

void SensoryMemory::makeNewRecordIfNotAlreadyPresent(HumanBase* const opponent)
{
	//else check to see if this Opponent already exists in the memory. If it doesn't,
	//create a new record
	if (_memory_map.find(opponent) == _memory_map.end())
	{
		_memory_map[opponent] = MemoryRecord();
	}
}

//------------------------ removeBotFromMemory --------------------------------
//
//  this removes a bot's record from memory
//-----------------------------------------------------------------------------
void SensoryMemory::removeBotFromMemory(HumanBase* const bot)
{
	MemoryMap::iterator record = _memory_map.find(bot);

	if (record != _memory_map.end())
	{
		_memory_map.erase(record);
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
	auto elist = _owner->getGame()->getNeighborsOnMove(_owner->getWorldPosition(), _viewRange);

	for (auto iter = std::begin(elist); iter != std::end(elist); iter++)
	{
		if (_owner->getTag() != (*iter)->getTag())
		{
			if (!isMasked((*iter)->getFamilyMask(), FamilyMask::HUMAN_BASE))
				continue;

			HumanBase* human = static_cast<HumanBase*>(*iter);

			//make sure it is part of the memory map
			makeNewRecordIfNotAlreadyPresent(human);

			//get a reference to this bot's data
			MemoryRecord& info = _memory_map[human];

			if(_owner->getWorldPosition().getDistance(human->getWorldPosition()) < _viewRange)
			{
				info.time_last_sensed = std::chrono::system_clock::now().time_since_epoch();
				info.last_sensed_position = (*iter)->getWorldPosition();
				info.time_last_visible = std::chrono::system_clock::now().time_since_epoch();

				if (_owner->getWorldPosition().getDistance(human->getWorldPosition()) < _attackRange)
				{
					info.attackable = true;
				}
				else
					info.attackable = false;

				if (info.within_view == false)
				{
					info.within_view = true;
					info.time_became_visible = info.time_last_sensed;
				}
			}
			else
			{
				info.attackable = false;
				info.within_view = false;
			}
		}
	}
}


//------------------------ getListOfRecentlySensedOpponents -------------------
//
//  returns a list of the bots that have been sensed recently
//-----------------------------------------------------------------------------
std::list<HumanBase*>
SensoryMemory::getListOfRecentlySensedOpponents()const
{
	//this will store all the opponents the bot can remember
	std::list<HumanBase*> opponents;

	std::chrono::duration<double> current_time = std::chrono::system_clock::now().time_since_epoch();

	MemoryMap::const_iterator rec = _memory_map.begin();
	for (rec; rec != _memory_map.end(); ++rec)
	{
		//if this bot has been updated in the memory recently, add to list
		if ((current_time - rec->second.time_last_sensed) <= _memory_span)
		{
			opponents.push_back(rec->first);
		}
	}

	return opponents;
}

//----------------------------- isOpponentAttackable --------------------------------
//
//  returns true if the bot given as a parameter can be shot (ie. its not
//  obscured by walls)
//-----------------------------------------------------------------------------
bool SensoryMemory::isOpponentAttackable(HumanBase* const opponent)const
{
	MemoryMap::const_iterator it = _memory_map.find(opponent);

	if (it != _memory_map.end())
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
	MemoryMap::const_iterator it = _memory_map.find(opponent);

	if (it != _memory_map.end())
	{
		return it->second.within_view;
	}

	return false;
}

//---------------------------- getLastRecordedPositionOfOpponent -------------------
//
//  returns the last recorded position of the bot
//-----------------------------------------------------------------------------
Vec2 SensoryMemory::getLastRecordedPositionOfOpponent(HumanBase* const opponent) const
{
	MemoryMap::const_iterator it = _memory_map.find(opponent);

	if (it != _memory_map.end())
	{
		return it->second.last_sensed_position;
	}

	throw std::runtime_error("< SensoryMemory::getLastRecordedPositionOfOpponent>: Attempting to get position of unrecorded bot");
}

//----------------------------- getTimeOpponentHasBeenVisible ----------------------
//
//  returns the amount of time the given bot has been visible
//-----------------------------------------------------------------------------
std::chrono::duration<double> SensoryMemory::getTimeOpponentHasBeenVisible(HumanBase* const opponent) const
{
	MemoryMap::const_iterator it = _memory_map.find(opponent);

	if (it != _memory_map.end() && it->second.within_view)
	{
		return std::chrono::system_clock::now().time_since_epoch() - it->second.time_became_visible;
	}

	return std::chrono::duration<double>(0);
}

int SensoryMemory::getDamage(HumanBase* const opponent)const
{
	MemoryMap::const_iterator it = _memory_map.find(opponent);

	if (it != _memory_map.end())
	{
		return it->second.recent_damage;
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
	MemoryMap::const_iterator it = _memory_map.find(opponent);

	if (it != _memory_map.end())
	{
		return std::chrono::system_clock::now().time_since_epoch() - it->second.time_last_visible;
	}

	return std::chrono::duration<double>(std::numeric_limits<double>::max());
}

//------------------------ getTimeSinceLastSensed ----------------------
//
//  returns the amount of time the given bot has been visible
//-----------------------------------------------------------------------------
std::chrono::duration<double> SensoryMemory::getTimeSinceLastSensed(HumanBase* const opponent)const
{
	MemoryMap::const_iterator it = _memory_map.find(opponent);

	if (it != _memory_map.end() && it->second.within_view)
	{
		return std::chrono::system_clock::now().time_since_epoch() - it->second.time_last_sensed;
	}

	return std::chrono::duration<double>(0);
}

bool SensoryMemory::isUnderAttack() const
{
	MemoryMap::const_iterator rec = _memory_map.begin();
	for (rec; rec != _memory_map.end(); ++rec)
	{
		//if this bot has hit us, return true
		if ((*rec).second.recent_damage > 0)
			return true;
	}
	return false;
}