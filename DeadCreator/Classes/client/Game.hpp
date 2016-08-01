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
#include "EntityHuman.hpp"
#include "SoundSource.hpp"
#include "StringHelper.hpp"
#include "SizeProtocol.h"
#include "MessageDispatcher.hpp"
#include "MessageNode.hpp"
#include "ObjectManager.hpp"
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
        
        class GameObject;
        class EntityHuman;
        class TriggerSystem;
        class RenderingSystem;
        class LogicStream;
        class GameResource;
        
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
            
            // game resource
            void loadGMXFile(const std::string& path);
            GameResource* getGameResource() const { return _gameResource; }
            
            // entity manager
            ObjectManager* _objectManager;
            
            void setPlayer(EntityHuman* player) { _player = player; }
            EntityHuman* getPlayerPtr() const { return _player; }
            const std::map<int, GameObject*>& getEntities() const { return _entities; }
            CellSpacePartition* getCellSpace() const { return _cellSpace; }
            void addEntity(GameObject* entity, int zOrder, int id);
            void removeEntity(int id);
            GameObject* getEntityFromID(int ID);
            int getNextValidID() { static int nextValidID = 0; return nextValidID++; }
            
            // rendering system
            RenderingSystem* getRenderingSysetm() const { return _renderingSystem; }
            
            
            std::list<GameObject*> getNeighborsOnMove(const cocos2d::Vec2& position, float speed) const;
            std::list<GameObject*> getNeighborsOnAttack(const cocos2d::Vec2& position, const cocos2d::Vec2& dir, float range) const;
            std::vector<Polygon> getNeighborWalls(const cocos2d::Vec2& position, float speed) const;
            std::vector<Polygon> getNeighborWalls(const cocos2d::Vec2& position, const cocos2d::Size screenSize) const;
            std::vector<Polygon> getNeighborWalls(const cocos2d::Vec2& position, const Segment& ray) const;
            
            
            // load operation
            void loadResource(const std::string& filePath);
            void loadUiLayer();
            
            TileType getStepOnTileType(const cocos2d::Vec2& pos);
            
            void sendMessage(double delaySeconds, MessageNode* receiver, MessageNode* sender, MessageType type, void* extraInfo);
            
            void pushLogic(double delaySeconds, MessageType type, void* extraInfo);
            bool isPaused() const { return _isPaused; }
            void pauseGame() { _isPaused = true; }
            void resumeGame() { _isPaused = false; }
            
        private:
            
            cocos2d::Size _winSize;
            SimpleReleasePool _releasePool;
            
            // map data
            GameResource* _gameResource;
            
            // entitiy manager
            EntityHuman* _player;
            std::map<int, GameObject*> _entities;
            CellSpacePartition* _cellSpace;
            
            // trigger system
            TriggerSystem* _triggerSystem;
            
            // rendering system
            RenderingSystem* _renderingSystem;
            
            // game
            LogicStream* _logicStream;
            bool _isPaused;
            int _bgmID;
            MessageDispatcher* _messenger;
            
        };
        
    }
}









