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
        
        class Camera2D
        {
            
        public:
            
            Camera2D() = default;
            virtual ~Camera2D() = default;
            
            void setCameraPos(const cocos2d::Vec2& pos) { _cameraPosition = pos; }
            cocos2d::Vec2 getCameraPos() const { return _cameraPosition; }
            
        private:
            
            cocos2d::Vec2                       _cameraPosition;
            
        };
        
    } // client
} // realtrick









