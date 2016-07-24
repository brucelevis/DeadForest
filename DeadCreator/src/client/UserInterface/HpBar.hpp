//
//  HpBar.hpp
//  TheDeadForest
//
//  Created by mac on 2016. 5. 16..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    
    class GameManager;
    
    namespace userinterface
    {
        
        class HpBar : public cocos2d::Node
        {
            
        public:
            
            static HpBar* create(GameManager* mgr);
            
            virtual bool init() override;
            
            explicit HpBar(GameManager* mgr);
            
            virtual ~HpBar() = default;
            
            void setHitPoint(float h);
            
        private:
            
            GameManager* _gameMgr;
            
            cocos2d::Sprite* _bundle;
            
            cocos2d::Sprite* _bar;
            
        };
    }
    
}