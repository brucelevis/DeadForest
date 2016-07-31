//
//  HpBar.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 5. 16..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    namespace client
    {
        
        class Game;
        
        class HpBar : public cocos2d::Node
        {
            
        public:
            
            explicit HpBar(Game* game);
            virtual ~HpBar() = default;
            
            static HpBar* create(Game* game);
            virtual bool init() override;
            
            void setHitPoint(float h);
            
        private:
            
            Game* _game;
            cocos2d::Sprite* _bundle;
            cocos2d::Sprite* _bar;
            
        };
        
    }
}









