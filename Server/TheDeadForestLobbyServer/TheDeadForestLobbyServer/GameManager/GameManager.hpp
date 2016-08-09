#pragma once

#include "Room.h"
#include "CellSpacePartition.hpp"
#include "GameMap.hpp"
#include "EntityHuman.hpp"

#define Z_ORDER_GAME_MAP    0
#define Z_ORDER_SHADOW      1
#define Z_ORDER_LIGHT       2
#define Z_ORDER_ITEMS       3
#define Z_ORDER_HUMAN       4
#define Z_ORDER_UI          10


#define BF_MULTIPLY         { GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA }
#define BF_ADDITIVE         { GL_ONE, GL_ONE }

namespace realtrick
{
	class Room;
	class EntityBase;
	class EntityHuman;
	class GameWorld;
	class RenderTarget;

    class GameManager
    {
    public:

		GameManager(Room* room) :
			_room(room),
			_nextValidID(0),
			_gameMap(nullptr)
		{};

		~GameManager() {};

		void update(float dt);

		void loadGameMap(const char* fileName);

		const std::map<int, EntityBase*>&       getEntities() const { return _entities; }

		GameMap*                                getGameMap() const { return _gameMap; }

		CellSpacePartition*                     getCellSpace() const { return _cellSpace; }

		int                                     getNextValidID() { return _nextValidID++; }

		std::list<EntityBase*>                  getNeighborsOnMove(const Vec2& position, float speed) const;

		std::list<EntityBase*>                  getNeighborsOnAttack(const Vec2& position, const Vec2& dir, float range) const;

		std::vector<geometry::Polygon>          getNeighborWalls(const Vec2& position, float speed) const;

		std::vector<geometry::Polygon>          getNeighborWalls(const Vec2& position, const Size screenSize) const;

		std::vector<geometry::Polygon>          getNeighborWalls(const Vec2& position, const geometry::Segment& ray) const;

		void clear();

		void addDynamicEntity(EntityBase* entity, int zOrder, int id);

		void removeEntity(int id);

		EntityBase* getEntityFromID(int ID);

        void moveJoystick(FlatbufferPacket *packet, int pid);
        
        void adjustPosition();

		void setLoadGamePlayer(int vid);

		void sendGameStart();

		bool isAllReady();

		template <typename T> static inline std::string _to_string(T number)
		{
			std::ostringstream convStream;
			convStream << number;
			return convStream.str();
		}

	private:

		Room*										_room;

		std::map<int, EntityBase*>                  _entities;

		CellSpacePartition*                         _cellSpace;

		GameMap*                                    _gameMap;
		
		int			                                _nextValidID;
    };
    
}












