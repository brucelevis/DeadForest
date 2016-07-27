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
using namespace rapidjson;

#include "GMXFile_generated.h"

namespace realtrick
{
    
    class GameManager;
    
    
    struct NameCoord
    {
        std::string name;
        cocos2d::Vec2 pos;
        
        NameCoord()
        {
            pos = cocos2d::Vec2::ZERO;
        }
        
        NameCoord(std::string name, cocos2d::Vec2 pos) : name(name), pos(pos)
        {}
        
        NameCoord(const NameCoord& rhs) : name(rhs.name), pos(rhs.pos)
        {}
        
        NameCoord operator=(const NameCoord& rhs)
        {
            if(&rhs == this)
                return *this;
            name = rhs.name;
            pos = rhs.pos;
            return *this;
        }
        
    };
    
    struct NameCoordAmount
    {
        std::string name;
        cocos2d::Vec2 pos;
        int amount;
        
        NameCoordAmount()
        {
            pos = cocos2d::Vec2::ZERO;
        }
        
        NameCoordAmount(std::string name, cocos2d::Vec2 pos, int amount) : name(name), pos(pos), amount(amount)
        {}
        
        NameCoordAmount(const NameCoordAmount& rhs) : name(rhs.name), pos(rhs.pos), amount(rhs.amount)
        {}
        
        NameCoordAmount operator=(const NameCoordAmount& rhs)
        {
            if(&rhs == this)
                return *this;
            name = rhs.name;
            pos = rhs.pos;
            amount = rhs.amount;
            return *this;
        }
    };
    
    class GameMap : public cocos2d::Node
    {
        
    public:
        
        enum class TileType { GRASS, DIRT, WATER };
        
    public:
        
        static GameMap* createWithGMXFile(GameManager* gameMgr, const DeadCreator::GMXFile* file);
        
        virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
        
        void updateChunk(const cocos2d::Vec2& position);
        TileType getStepOnTileType(const cocos2d::Vec2& pos);
        
        int getTileWidth() const                                                    { return _tileWidth; }
        int getTileHeight() const                                                   { return _tileHeight; }
        int getWorldWidth() const                                                   { return _worldWidth; }
        int getWorldHeight() const                                                  { return _worldHeight; }
        int getCellWidth() const                                                    { return _cellWidth; }
        int getCellHeight() const                                                   { return _cellHeight; }
        
        const std::vector<Polygon>& getCollisionData() const                        { return _collisionData; }
        const std::vector<std::vector<std::string>>& getTileData() const            { return _tileData; }
        
        cocos2d::Vec2 getWorldPosition() const                                      { return _worldPosition; }
        void setWorldPosition(const cocos2d::Vec2& worldPos)                        { _worldPosition = worldPos; }
        
        std::vector<Segment> getNeighborWall(const cocos2d::Vec2 position) const;
        
    private:
        
        GameManager* _gameMgr;
        
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
        std::vector<std::vector<std::string>> _tileData;
        cocos2d::Vec2 _worldPosition;
        
        bool _isNormal;
        
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





