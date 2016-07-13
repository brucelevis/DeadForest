//
//  GameMap.hpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 12..
//  
//


#pragma once

#include <string>
#include <exception>

#include "cocos2d.h"
#include "Physics.hpp"
#include "ParamLoader.hpp"
#include "json/document.h"
#include "ParseStructures.hpp"
using namespace rapidjson;

#include "GMXFile_generated.h"

namespace realtrick
{
    
    class GameManager;
    
    
    class GameMap : public cocos2d::Node
    {
        
    public:
        
        enum class TileType { GRASS, DIRT, WATER };
        
    public:
        
        static GameMap* createWithGMXFile(GameManager* gameMgr, const DeadCreator::GMXFile* file);
        
        void updateChunk(const Vec2& position);
        
        int getTileWidth() const                                                    { return _tileWidth; }
        int getTileHeight() const                                                   { return _tileHeight; }
        int getWorldWidth() const                                                   { return _worldWidth; }
        int getWorldHeight() const                                                  { return _worldHeight; }
        int getCellWidth() const                                                    { return _cellWidth; }
        int getCellHeight() const                                                   { return _cellHeight; }
        
        const std::vector<Polygon>&              getCollisionData() const           { return _collisionData; }
        
        const std::vector<std::vector<std::string>>&       getTileData() const      { return _tileData; }
        
        const std::vector<cocos2d::Vec2>&       getStartingLocationData() const     { return _startingLocationList; }
        
        const std::vector<NameCoordAmount>&      getItemData() const                { return _itemData; }
        
        const std::vector<NameCoord>&            getIndoorTileData() const          { return _indoorTileData; }
        
        const std::vector<NameCoord>&            getWallsData() const               { return _wallsData; }
        
        const std::vector<NameCoord>&            getDoodadData() const              { return _doodadData; }

        virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
        
        cocos2d::Vec2 getWorldPosition() const                                      { return _worldPosition; }
        
        void setWorldPosition(const cocos2d::Vec2& worldPos)                        { _worldPosition = worldPos; }
        
        std::vector<Segment> getNeighborWall(const cocos2d::Vec2 position) const;
        
        TileType getStepOnTileType(const cocos2d::Vec2& pos);
        
    private:
        
        GameManager*                                    _gameMgr;
        
        std::vector<std::vector<Sprite*>>               _currTiles;
        
        int                                             _numOfTileX;
        int                                             _numOfTileY;
        int                                             _tileWidth;
        int                                             _tileHeight;
        int                                             _worldWidth;
        int                                             _worldHeight;
        
        int                                             _cellWidth;
        int                                             _cellHeight;
        
        int                                             _numOfViewableTileX;
        int                                             _numOfViewableTileY;
        
        std::pair<int, int>                             _pivotIndex;
        
        std::string                                     _data;
        
        rapidjson::Document                             _doc;
        
        std::vector<Polygon>                            _collisionData;
        
        std::vector<std::vector<std::string>>           _tileData;
        
        std::vector<Vec2>                               _startingLocationList;
        
        std::vector<NameCoordAmount>                    _itemData;
        
        std::vector<NameCoord>                          _indoorTileData;
        
        std::vector<NameCoord>                          _wallsData;
        
        std::vector<NameCoord>                          _doodadData;
        
        cocos2d::Vec2                                   _worldPosition;
        
        bool                                            _isNormal;
        
    private:
        
        explicit GameMap(GameManager* gameMgr);
        
        virtual ~GameMap();
        
        bool initGMXFile(const DeadCreator::GMXFile* file);
        
        inline TileType convertToTileType(char c);
        
    };
    
    
    inline GameMap::TileType GameMap::convertToTileType(char c)
    {
        TileType ret = TileType::DIRT;
        switch ( c )
        {
            case '1': ret = TileType::DIRT; break;
            case '2': ret = TileType::GRASS; break;
            case '3': ret = TileType::WATER; break;
            default: break;
        }
        return ret;
    }
    
    
}





