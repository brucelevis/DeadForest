//
//  GameMap.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 12..
//
//


#pragma once

#include <string>
#include <exception>

#include "cocos2d.h"
#include "Physics.hpp"
#include "ParamLoader.hpp"

#include "GMXFile_generated.h"
#include "EntityType.hpp"
#include "Tileset.hpp"

namespace realtrick
{
    namespace client
    {
        
        class Game;
        
        class GameMap : public cocos2d::Node
        {
            
        public:
            
            explicit GameMap(Game* game);
            virtual ~GameMap();
            bool initGMXFile(const DeadCreator::GMXFile* file);

            static GameMap* createWithGMXFile(Game* game, const DeadCreator::GMXFile* file);
            
            virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
            
            void updateChunk(const cocos2d::Vec2& position);
            TileType getStepOnTileType(const cocos2d::Vec2& pos);
            
            int getTileWidth() const { return _tileWidth; }
            int getTileHeight() const { return _tileHeight; }
            int getWorldWidth() const { return _worldWidth; }
            int getWorldHeight() const { return _worldHeight; }
            int getCellWidth() const { return _cellWidth; }
            int getCellHeight() const { return _cellHeight; }
            int getNumofTileX() const { return _numOfTileX; }
            int getNumofTileY() const { return _numOfTileY; }
            int getNumOfViewableTileX() const { return _numOfViewableTileX; }
            int getNumOfViewableTileY() const { return _numOfViewableTileY; }
            
            const std::vector<Polygon>& getCollisionData() const { return _collisionData; }
            const std::vector<std::vector<Tileset>>& getTileData() const { return _tileData; }
            
            cocos2d::Vec2 getWorldPosition() const { return _worldPosition; }
            void setWorldPosition(const cocos2d::Vec2& worldPos) { _worldPosition = worldPos; }
            
        private:
            
            Game* _game;
            
            std::vector<std::vector<cocos2d::Sprite*>> _currTiles;
            
            int _numOfTileX;
            int _numOfTileY;
            int _tileWidth;
            int _tileHeight;
            int _worldWidth;
            int _worldHeight;
            
            int _cellWidth;
            int _cellHeight;
            
            int _numOfViewableTileX;
            int _numOfViewableTileY;
            
            std::pair<int, int> _pivotIndex;
            
            std::vector<Polygon> _collisionData;
            std::vector<std::vector<Tileset>> _tileData;
            cocos2d::Vec2 _worldPosition;
            
            bool _isNormal;
            
        };
        
    }
}









