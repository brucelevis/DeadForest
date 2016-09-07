//
//  Camera2D.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 2. 14..
//
//

#pragma once

#include "cocos2d.h"
#include "EntityBase.hpp"

namespace realtrick
{
    namespace client
    {
        
        class Game;
        
        class Camera2D : public cocos2d::Node
        {
            
        public:
            
            Camera2D() = default;
            virtual ~Camera2D() = default;
            
            CREATE_FUNC(Camera2D);
            
            void setCameraPos(const cocos2d::Vec2& pos) { this->setPosition(pos); }
            cocos2d::Vec2 getCameraPos() const { return getPosition(); }
            
        };
        
    }
}









