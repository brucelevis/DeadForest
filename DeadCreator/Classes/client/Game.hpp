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
#include "GameMap.hpp"
#include "EntityHuman.hpp"
#include "SoundSource.hpp"
#include "StringHelper.hpp"
#include "SizeProtocol.h"
#include "MessageDispatcher.hpp"
#include "MessageNode.hpp"

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
        class EntityHuman;
        class Camera2D;
        class RenderTarget;
        class TriggerSystem;
        class UiLayer;
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
            void setPlayer(EntityHuman* player) { _player = player; }
            
            EntityHuman* getPlayerPtr() const { return _player; }
            const std::map<int, EntityBase*>& getEntities() const { return _entities; }
            GameMap* getGameMap() const { return _gameMap; }
            CellSpacePartition* getCellSpace() const { return _cellSpace; }
            
            static int getNextValidID() { static int _nextValidID = 0; return _nextValidID++; }
            
            std::list<EntityBase*> getNeighborsOnMove(const cocos2d::Vec2& position, float speed) const;
            std::list<EntityBase*> getNeighborsOnAttack(const cocos2d::Vec2& position, const cocos2d::Vec2& dir, float range) const;
            std::vector<Polygon> getNeighborWalls(const cocos2d::Vec2& position, float speed) const;
            std::vector<Polygon> getNeighborWalls(const cocos2d::Vec2& position, const cocos2d::Size screenSize) const;
            std::vector<Polygon> getNeighborWalls(const cocos2d::Vec2& position, const Segment& ray) const;
            
            void loadResource(const std::string& filePath);
            
            Camera2D* getGameCamera() const { return _gameCamera; }
            void drawCellSpaceDebugNode();
            
            cocos2d::Vec2 worldToLocal(const cocos2d::Vec2& p) const ;
            cocos2d::Vec2 worldToLocal(const cocos2d::Vec2& p, const cocos2d::Vec2& camera) const ;
            
            void addEntity(EntityBase* entity, int zOrder, int id);
            void removeEntity(int id);
            EntityBase* getEntityFromID(int ID);
            
            void pushLogic(double delaySeconds, MessageType type, void* extraInfo);
            
            void loadUiLayer();
            void setZoom(float r) { _zoomScale = r; _rootNode->setScale(r); }
            float getZoomScale() const { return _zoomScale; }
            
            bool isPaused() const { return _isPaused; }
            void pauseGame() { _isPaused = true; }
            void resumeGame() { _isPaused = false; }
            
            const std::map<std::string, cocos2d::Rect>& getLocationMap() const { return _locations; }
            
            // temp
            void loadGMXFile(const std::string& path);
            
        private:
            
            cocos2d::Size                               _winSize;
            
            // map data
            GameMap*                                    _gameMap;
            GameResource* _gameResource;
            
            // entitiy manager
            EntityHuman*                                _player;
            std::map<int, EntityBase*>                  _entities;
            CellSpacePartition*                         _cellSpace;
            
            // trigger system
            TriggerSystem*                              _triggerSystem;
            std::map<std::string, cocos2d::Rect>        _locations;
            
            // rendering system
            Camera2D*                                   _gameCamera;
            cocos2d::ClippingRectangleNode*             _clipNode;
            cocos2d::Node*                              _rootNode;
            UiLayer*                                    _uiLayer;
            float                                       _zoomScale;
            
            // game
            LogicStream*                                _logicStream;
            bool                                        _isPaused;
            int                                         _bgmID;
            
        };
        
    }
}









