//
//  Terrain.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 31..
//
//

#pragma once

#include "cocos2d.h"

#include "EntityBase.hpp"

namespace realtrick
{
    class TileImage;
    
    namespace client
    {
        
        class Game;
        
        class Terrain : public EntityBase
        {
            
        public:
            
            explicit Terrain(Game* game);
            virtual ~Terrain();
            
            static Terrain* create(Game* game);
            virtual bool init() override;
            
            void updateChunk(const cocos2d::Vec2& position);
            
        private:
            
            Game* _game;
            
            int _numOfViewableTileX;
            int _numOfViewableTileY;

            std::vector<std::vector<realtrick::TileImage*>> _currTiles;
            std::pair<int, int> _pivotIndex;
            
        };
        
    }
}
