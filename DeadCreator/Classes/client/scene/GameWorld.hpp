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
        class UiLayer;
        class LogicStream;
        
        class GameWorld : public cocos2d::Layer
        {
            
        public:
            
            GameWorld();
            virtual ~GameWorld();
            
            static cocos2d::Scene* createScene();
            virtual bool init() override;
            CREATE_FUNC(GameWorld);
            
            virtual void update(float dt) override;
            
            cocos2d::Size getWorldSize() const { return _winSize; }
            
            LogicStream* getLogicStream() const { return _logicStream; }
            void seteLogicStream(LogicStream* stream) { _logicStream = stream; }
            
            void pushLogic(double delaySeconds, MessageType type, void* extraInfo);

            
        private:
            
            cocos2d::Size                       _winSize;
            GameManager*                        _gameMgr;
            LogicStream*                        _logicStream;
            
        };
        
    }
}









