//
//  Game.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 27..
//
//

#pragma once

#include <AudioEngine.h>

#include "cocos2d.h"
#include "CellSpacePartition.hpp"
#include "EntityPlayer.hpp"
#include "SoundSource.hpp"
#include "StringHelper.hpp"
#include "SizeProtocol.h"
#include "MessageDispatcher.hpp"
#include "MessageNode.hpp"
#include "SimpleReleasePool.hpp"

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
    namespace client
    {
        
        class EntityBase;
        class EntityPlayer;
        class TriggerSystem;
        class RenderingSystem;
        class LogicStream;
        class GameResource;
        class EntityManager;
        
        class Game : public cocos2d::Node
        {
            
        public:
            
            Game();
            virtual ~Game();
            
            bool init() override;
            static Game* create();
            static cocos2d::Scene* createScene();
            
            void clear();
            void update(float dt) override;
            
            GameResource* getGameResource() const { return _gameResource; }
            EntityManager* getEntityManager() const { return _entityManager; }
            RenderingSystem* getRenderingSysetm() const { return _renderingSystem; }
            CellSpacePartition* getCellSpace() const { return _cellSpace; }
            
            // entity helper
            void addEntity(EntityBase* ent, int zOrder = 0);
            void removeEntity(EntityBase* ent);
            EntityPlayer* getPlayerPtr() const;
            
            void loadGMXFile(const std::string& path);
            void loadUiLayer();
            
            std::list<EntityBase*> getNeighborsOnMove(const cocos2d::Vec2& pos, float speed) const;
            std::list<EntityBase*> getNeighborsOnAttack(const cocos2d::Vec2& pos, const cocos2d::Vec2& dir, float range) const;
            std::vector<Polygon> getNeighborWalls(const cocos2d::Vec2& pos, float speed) const;
            std::vector<Polygon> getNeighborWalls(const cocos2d::Vec2& pos, const cocos2d::Size screenSize) const;
            std::vector<Polygon> getNeighborWalls(const cocos2d::Vec2& pos, const Segment& ray) const;
            
            TileType getStepOnTileType(const cocos2d::Vec2& pos);
            
            void sendMessage(double delaySeconds, MessageNode* receiver, MessageNode* sender, MessageType type, void* extraInfo);
            
            void pushLogic(double delaySeconds, MessageType type, void* extraInfo);
            bool isPaused() const { return _isPaused; }
            void pauseGame() { _isPaused = true; }
            void resumeGame() { _isPaused = false; }
            
            void addLog(const std::string& log);
            void clearLogs();
            const std::string& getLogString() const { return _logString; }
            bool& isLogAdded() { return _isLogAdded; }
            
        private:
            
            cocos2d::Size _winSize;
            SimpleReleasePool _releasePool;
            
            // map data
            GameResource* _gameResource;
            
            // entitiy manager
            EntityManager* _entityManager;
            
            // cell space
            CellSpacePartition* _cellSpace;
            
            // trigger system
            TriggerSystem* _triggerSystem;
            
            // rendering system
            RenderingSystem* _renderingSystem;
            
            // message system
            MessageDispatcher* _messenger;
            
            LogicStream* _logicStream;
            
            bool _isPaused;
            int _bgmID;
            
            // stored logs
            std::vector<std::pair<std::string, int>> _logs;
            std::string _logString;
            bool _isLogAdded = false;
            
        };
        
    }
}









