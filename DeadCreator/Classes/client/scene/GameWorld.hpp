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
        
        class GameWorld : public cocos2d::Layer
        {
            
        public:
            
            GameWorld();
            virtual ~GameWorld();
            
            static cocos2d::Scene* createScene();
            virtual bool init() override;
            CREATE_FUNC(GameWorld);
            
            virtual void update(float dt) override;
            
            GameManager* getGameManager() const { return _gameMgr; }
            
        private:
            
            cocos2d::Size                       _winSize;
            GameManager*                        _gameMgr;
            
        };
        
    }
}









