//
//  GameManager.hpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 27..
//
//

#pragma once

#include <string>
#include <exception>
#include <memory>
#include <AudioEngine.h>

#include "cocos2d.h"
#include "CellSpacePartition.hpp"
#include "GameMap.hpp"
#include "EntityHuman.hpp"
#include "SoundSource.hpp"

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
    
    class EntityBase;
    class EntityHuman;
    class GameWorld;
    class Camera2D;
    class RenderTarget;
    
    class GameManager
    {
        
    public:
        
        explicit GameManager(GameWorld* world);
        
        virtual ~GameManager();
        
        void clear();
        void update(float dt);
        void setPlayer(EntityHuman* player);
        
        EntityHuman* getPlayerPtr() const { return _player; }
        const std::map<int, EntityBase*>& getEntities() const { return _entities; }
        GameMap* getGameMap() const { return _gameMap; }
        CellSpacePartition* getCellSpace() const { return _cellSpace; }
        static int getNextValidID() { return _nextValidID++; }
        GameWorld* getGameWorld() const { return _gameWorld; }
        
        std::list<EntityBase*> getNeighborsOnMove(const cocos2d::Vec2& position, float speed) const;
        std::list<EntityBase*> getNeighborsOnAttack(const cocos2d::Vec2& position, const cocos2d::Vec2& dir, float range) const;
        std::vector<Polygon> getNeighborWalls(const cocos2d::Vec2& position, float speed) const;
        std::vector<Polygon> getNeighborWalls(const cocos2d::Vec2& position, const cocos2d::Size screenSize) const;
        std::vector<Polygon> getNeighborWalls(const cocos2d::Vec2& position, const Segment& ray) const;
        
        void loadSingleGameMap(const char* fileName);
        void loadMultiGameMap(const char* fileName);
        void loadGMXFile(const std::string& filePath);
        
        void setGameCamera(Camera2D* camera) { _gameCamera = camera; }
        Camera2D* getGameCamera() const { return _gameCamera; }
        cocos2d::DrawNode* getDebugNode() const { return _debugNode; }
        
        cocos2d::Vec2 worldToLocal(const cocos2d::Vec2& p) const ;
        cocos2d::Vec2 worldToLocal(const cocos2d::Vec2& p, const cocos2d::Vec2& camera) const ;
        
        int getRemainHuman() const;
        
        std::string getCurrentLocation() const;
        
        void addDynamicEntity(EntityBase* entity, int zOrder, int id);
        void addStaticEntity(cocos2d::Node* entity, int zOrder, int id);
        void removeEntity(int id);
        EntityBase* getEntityFromID(int ID);
        
        void pushLogic(double delaySeconds, MessageType type, void* extraInfo);
        
        template <typename T> static inline std::string _to_string(T number)
        {
            std::ostringstream convStream;
            convStream << number;
            return convStream.str();
        }
        
    private:
        
        void _setGameWorld(GameWorld* world)        { _gameWorld = world; }
        
    private:
        
        GameWorld*                                  _gameWorld;
        cocos2d::Size                               _winSize;
        EntityHuman*                                _player;
        std::map<int, EntityBase*>                  _entities;
        CellSpacePartition*                         _cellSpace;
        GameMap*                                    _gameMap;
        Camera2D*                                   _gameCamera;
        cocos2d::DrawNode*                          _debugNode;
        static int                                  _nextValidID;
    
    };
    
}












