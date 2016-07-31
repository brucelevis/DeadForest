//
//  RenderingSystem.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    namespace client
    {
     
        class Game;
        class Camera2D;
        class TileImage;
        class Terrain;
        
        class RenderingSystem : public cocos2d::Node
        {
            
        public:
            
            explicit RenderingSystem(Game& game);
            virtual ~RenderingSystem();
            
            static RenderingSystem* create(Game& game);
            bool init(Game& game);
            
        private:
            
            Game& _game;
            Camera2D* _camera;
            Terrain* terrain;
            
        };
        
    }
}