//
//  GameResource.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 31..
//
//

#pragma once

#include "cocos2d.h"
#include "Physics.hpp"
#include "ParamLoader.hpp"

#include "GMXFile_generated.h"
#include "Infos.hpp"
#include "Tileset.hpp"


namespace realtrick
{
    namespace client
    {
        
        class TriggerDataBase;
        
        struct TriggerData
        {
            std::vector<int> players;
            std::vector<TriggerDataBase*> conditions;
            std::vector<TriggerDataBase*> actions;
        };
        
        class GameResource : public cocos2d::Ref
        {
            
            friend class TriggerSystem;
            friend class EntityManager;
            friend class Game;
            
        public:
        
            virtual ~GameResource() = default;
            
            static GameResource* createWithGMXFile(const std::string& path)
            {
                auto ret = new (std::nothrow) GameResource();
                if ( ret && ret->initGMXFile(path) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            static GameResource* createWithBinary(const char* buffer)
            {
                auto ret = new (std::nothrow) GameResource();
                if ( ret && ret->initWithBinary(buffer) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool initGMXFile(const std::string& path);
            bool initWithBinary(const char* buffer);
            
            // this is constant datas so data that from "getter" is available in game.
            // hence mutable datas (such as entities, triggers) do not have to make getter. (just use for initialize game)
            const int getTileWidth() const { return _tileWidth; }
            const int getTileHeight() const { return _tileHeight; }
            const int getWorldWidth() const { return _worldWidth; }
            const int getWorldHeight() const { return _worldHeight; }
            const int getCellWidth() const { return _cellWidth; }
            const int getCellHeight() const { return _cellHeight; }
            const int getNumOfTileX() const { return _numOfTileX; }
            const int getNumOfTileY() const { return _numOfTileY; }
            
            const std::vector<realtrick::Polygon>& getCollisionData() const { return _collisionData; }
            const std::vector<realtrick::Polygon>& getSimpleCollisionData() const { return _simpleCollisionData; }
            const std::vector<std::vector<Tileset>>& getTileData() const { return _tileData; }
            const std::map<std::string, cocos2d::Rect>& getLocations() const { return _locations; }
            
            void updateLocation(const std::string& key, const cocos2d::Rect& rect);
            
        private:
            
            GameResource() = default;
            
        private:
            
            // terrain data
            int _defaultTile;
            
            int _numOfTileX;
            int _numOfTileY;
            
            int _tileWidth;
            int _tileHeight;
            
            int _worldWidth;
            int _worldHeight;
            
            // cell space
            int _cellWidth;
            int _cellHeight;
            
            // collision, tile
            std::vector<realtrick::Polygon> _collisionData;
            std::vector<realtrick::Polygon> _simpleCollisionData;
            std::vector<std::vector<Tileset>> _tileData;
            
            // entity
            std::vector<EntityData> _entities;
            
            // location
            std::map<std::string, cocos2d::Rect> _locations;
            
            // trigger
            std::vector<TriggerData> _triggers;
            
            // force info
            ForceInfo _forces[4];
            
            // player info
            PlayerInfo _playerInfos[9];
            
            // switchs
            Switch _switchs[256];
            
        };
        
    }
}









