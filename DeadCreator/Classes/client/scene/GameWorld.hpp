//
//  GameWorld.hpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 12..
//
//


#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "MessageNode.hpp"
#include "MessageTypes.hpp"

namespace realtrick
{
    namespace client
    {
        
        class GameManager;
        class EntityHuman;
        class Camera2D;
        class ItemBase;
        class RenderTarget;
        class UiLayer;
        class LogicStream;
        
        class GameWorld : public cocos2d::Layer
        {
            
        public:
            
            GameWorld() : _isPaused(true) {}
            virtual ~GameWorld();
            
            static cocos2d::Scene* createScene();
            virtual bool init() override;
            CREATE_FUNC(GameWorld);
            
            virtual void update(float dt) override;
            
            Camera2D* getGameCamera() const { return _gameCamera; }
            cocos2d::Size getWorldSize() const { return _winSize; }
            GameManager* getGameManager() const { return _gameMgr; }
            EntityHuman* getPlayerPtr() const { return _player; }
            RenderTarget* getRenderTarget() const { return _renderTarget; }
            
            void setPlayerPtr(EntityHuman* player) { _player = player; }
            
            void loadGameDataByNetwork();
            void loadGameDataByFile();
            
            void setZoom(float r);
            
            LogicStream* getLogicStream() const { return _logicStream; }
            void seteLogicStream(LogicStream* stream) { _logicStream = stream; }
            
            void pushLogic(double delaySeconds, MessageType type, void* extraInfo);
            
            UiLayer* getUiLayer() const { return _uiLayer; }
            
            void pauseGame();
            void resumeGame();
            bool isPaused() const { return _isPaused; }
            
            void loadUiLayer();
            
        private:
            
            cocos2d::ClippingRectangleNode*     _root;
            
            cocos2d::Size                       _winSize;
            GameManager*                        _gameMgr;
            Camera2D*                           _gameCamera;
            EntityHuman*                        _player;
            RenderTarget*                       _renderTarget;
            UiLayer*                            _uiLayer;
            LogicStream*                        _logicStream;
            bool                                _isPaused;
            
        };
        
    }
}









