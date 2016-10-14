#ifndef SENSORY_SYSTEM_H
#define SENSORY_SYSTEM_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name: SensoryMemory.h
//
//  Author: insub im (insooneelife@naver.com)
//
//  Desc:
//
//-----------------------------------------------------------------------------
#include <map>
#include <list>
#include <sstream>
#include "cocos2d.h"

namespace realtrick
{
	namespace client
	{
		class HumanBase;
		class ItemBase;

		class MemoryRecord
		{
		public:

			//records the time the opponent was last sensed (seen or heard). This
			//is used to determine if a bot can 'remember' this record or not. 
			//(if CurrentTime() - time_last_sensed is greater than the bot's
			//memory span, the data in this record is made unavailable to clients)
			std::chrono::duration<double>			time_last_sensed;

			//it can be useful to know how long an opponent has been visible. This 
			//variable is tagged with the current time whenever an opponent first becomes
			//visible. It's then a simple matter to calculate how long the opponent has
			//been in view (CurrentTime - time_became_visible)
			std::chrono::duration<double>			time_became_visible;

			//it can also be useful to know the last time an opponent was seen
			std::chrono::duration<double>			time_last_visible;

			//a vector marking the position where the opponent was last sensed. This can
			// be used to help hunt down an opponent if it goes out of view
			cocos2d::Vec2	last_sensed_position;

			//set to true if opponent is within the field of view of the owner
			bool			within_view;

			//set to true if there is no obstruction between the opponent and the owner, 
			//permitting a shot.
			bool			attackable;

			int				recent_damage;


			MemoryRecord()
				:
				time_last_sensed(-999),
				time_became_visible(-999),
				time_last_visible(0),
				within_view(false),
				attackable(false),
				recent_damage(0)
			{}

			friend std::stringstream & operator<<(std::stringstream &ss, const MemoryRecord& b);
		};


		class SensoryMemory
		{
		private:

			typedef std::map<HumanBase*, MemoryRecord> MemoryMap;

			//the owner of this instance
			HumanBase* const	_owner;

			//this container is used to simulate memory of sensory events. A MemoryRecord
			//is created for each opponent in the environment. Each record is updated 
			//whenever the opponent is encountered. (when it is seen or heard)
			MemoryMap			_memory_map;
			std::vector<ItemBase*> _sensedItems;


			//a bot has a memory span equivalent to this value. When a bot requests a 
			//list of all recently sensed opponents this value is used to determine if 
			//the bot is able to remember an opponent or not.
			std::chrono::duration<double>				_memory_span;

			double _viewRange;
			double _attackRange;

			//this methods checks to see if there is an existing record for bot. If
			//not a new MemoryRecord record is made and added to the memory map.(called
			//by updateWithSoundSource & updateVision)
			void       makeNewRecordIfNotAlreadyPresent(HumanBase* bot);

		public:

			SensoryMemory(HumanBase* const owner, double memory_span);

			~SensoryMemory();

			void setAttackRange(int range) { _attackRange = range; }
			int getAttackRange() const { return _attackRange; }
			int getViewRange() const { return _viewRange; }

			//this removes a bot's record from memory
			void	removeBotFromMemory(HumanBase* const bot);
			void	removeItemFromMemory(ItemBase* const item);

			//this method iterates through all the opponents in the game world and 
			//updates the records of those that are in the owner's FOV(field of view)
			void	updateVision();

			//this method renders the infomation about sensory memory
			void	render();

			bool			isOpponentAttackable(HumanBase* const opponent)const;
			bool			isOpponentWithinFOV(HumanBase* const opponent)const;
			cocos2d::Vec2	getLastRecordedPositionOfOpponent(HumanBase* const opponent)const;
			std::chrono::duration<double>			getTimeOpponentHasBeenVisible(HumanBase* const opponent)const;
			std::chrono::duration<double>			getTimeSinceLastSensed(HumanBase* const opponent)const;
			std::chrono::duration<double>			getTimeOpponentHasBeenOutOfView(HumanBase* const opponent)const;
			int				getDamage(HumanBase* const opponent)const;
			bool			isUnderAttack() const;

			cocos2d::Vec2 avoidingEnemiesVector(cocos2d::Vec2& pos, cocos2d::Vec2& heading);
			cocos2d::Vec2 avoidingEnemiesVector(HumanBase* const owner);

			// Returns enemy list
			std::list<HumanBase*> getListOfRecentlySensedOpponents() const;

			const std::vector<ItemBase*>& getSensedItems() const;

			std::vector<ItemBase*> queryMeleeWeapon();
			std::vector<ItemBase*> queryUsableRangeWeapon();
			std::vector<ItemBase*> queryBullets();
			bool isReadyToFight() const;

		};
	}
}
#endif