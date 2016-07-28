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
    namespace client
    {
        
        class GameManager;
        
        class HpBar : public cocos2d::Node
        {
            
        public:
            
            explicit HpBar(GameManager* mgr);
            virtual ~HpBar() = default;
            
            static HpBar* create(GameManager* mgr);
            virtual bool init() override;
            
            void setHitPoint(float h);
            
        private:
            
            GameManager* _gameMgr;
            cocos2d::Sprite* _bundle;
            cocos2d::Sprite* _bar;
            
        };
        
    }
}









