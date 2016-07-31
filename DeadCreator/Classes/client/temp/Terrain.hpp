//
//  Terrain.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    class TileImage;
    
    namespace client
    {
        
        class RenderingSystem;
        class GameResource;
        
        class Terrain : public cocos2d::Node
        {
            
        public:
            
            explicit Terrain(RenderingSystem* renderSys);
            virtual ~Terrain();
            
            static Terrain* create(RenderingSystem* renderSys, GameResource* res);
            bool init(GameResource* res);
            virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
            
            void updateChunk(const cocos2d::Vec2& position);
            
        private:
            
            RenderingSystem* _renderSys;
            GameResource* _gameResource;
            
            cocos2d::Vec2 _worldPosition;
            
            int _numOfViewableTileX;
            int _numOfViewableTileY;

            std::vector<std::vector<realtrick::TileImage*>> _currTiles;
            std::pair<int, int> _pivotIndex;
            
        };
        
    }
}