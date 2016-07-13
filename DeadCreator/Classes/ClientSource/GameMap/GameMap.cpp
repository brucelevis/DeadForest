//
//  GameMap.cpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 12..
//  
//

#include <string>

#include "GameMap.hpp"
#include "GameManager.hpp"
#include "Camera2D.hpp"
#include "TileBase.hpp"


namespace realtrick
{
    
    GameMap::GameMap(GameManager* gameMgr) : _gameMgr(gameMgr),
    _numOfTileX(0),
    _numOfTileY(0),
    _sizeOfTile(0),
    _worldWidth(0),
    _worldHeight(0),
    _cellWidth(0),
    _cellHeight(0),
    _numOfViewableTileX(0),
    _numOfViewableTileY(0),
    _isNormal(false)
    {}
    
    
    GameMap::~GameMap()
    {}
    
    
    GameMap* GameMap::create(GameManager* gameMgr, const char* fileName)
    {
        GameMap* ret = new (std::nothrow) GameMap(gameMgr);
        if (ret && ret->initGameMap(fileName))
        {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    
    
    bool GameMap::initGameMap(const char* fileName)
    {
        if ( !Node::init() )
        {
            return false;
        }
        
        _numOfViewableTileX     = Prm.getValueAsInt("numOfViewableTileX");
        _numOfViewableTileY     = Prm.getValueAsInt("numOfViewableTileY");
        _sizeOfTile             = Prm.getValueAsInt("sizeOfTile");
        _worldWidth             = Prm.getValueAsInt("worldWidth");
        _worldHeight            = Prm.getValueAsInt("worldHeight");
        _cellWidth              = Prm.getValueAsInt("cellWidth");
        _cellHeight             = Prm.getValueAsInt("cellHeight");
        
        _currTiles.resize(_numOfViewableTileY);
        for (int i = 0; i < _numOfViewableTileY; ++i)
        {
            _currTiles[i].resize(_numOfViewableTileX);
        }
        
        for (int i = 0; i < _numOfViewableTileY; ++i)
        {
            for (int j = 0; j < _numOfViewableTileX; ++j)
            {
                _currTiles[i][j] = Sprite::create();
                addChild(_currTiles[i][j] , 1);
            }
        }
        
        _parseFromFile(fileName);
        
        return true;
    }
    
    
    GameMap* GameMap::createWithGMXFile(GameManager* gameMgr, const DeadCreator::GMXFile* file)
    {
        auto ret = new (std::nothrow) GameMap(gameMgr);
        if ( ret && ret->initGMXFile(file) )
        {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    
    
    bool GameMap::initGMXFile(const DeadCreator::GMXFile *file)
    {
        if ( !Node::init() )
            return false;
        
        _numOfViewableTileX     = Prm.getValueAsInt("numOfViewableTileX");
        _numOfViewableTileY     = Prm.getValueAsInt("numOfViewableTileY");
        _sizeOfTile             = file->tile_size()->width();
        _worldWidth             = file->tile_size()->width() * file->number_of_tiles()->x();
        _worldHeight            = file->tile_size()->height() * file->number_of_tiles()->y();
        _cellWidth              = file->cell_space_size()->width();
        _cellHeight             = file->cell_space_size()->height();
        
        _currTiles.resize(_numOfViewableTileY);
        for (int i = 0; i < _numOfViewableTileY; ++i)
        {
            _currTiles[i].resize(_numOfViewableTileX);
        }
        
        for (int i = 0; i < _numOfViewableTileY; ++i)
        {
            for (int j = 0; j < _numOfViewableTileX; ++j)
            {
                _currTiles[i][j] = Sprite::create();
                addChild(_currTiles[i][j] , 1);
            }
        }

        for ( auto poly = file->collision_regions()->begin() ; poly != file->collision_regions()->end() ; ++ poly )
        {
            Polygon p;
            for( auto vert = poly->vertices()->begin(); vert != poly->vertices()->end() ; ++ vert )
            {
                p.pushVertex(Vec2(vert->x(), vert->y()));
            }
            _collisionData.push_back(p);
            _gameMgr->getCellSpace()->addWall(p);
        }
        
        _numOfTileX = file->number_of_tiles()->x() + DUMMY_TILE_SIZE * 2;
        _numOfTileY = file->number_of_tiles()->y() * 2 + DUMMY_TILE_SIZE * 4;
        
        _tileData.resize(_numOfTileY);
        for(int i = 0 ; i < _numOfTileY; ++ i)
        {
            _tileData[i].resize(_numOfTileX);
        }
        
        auto defaultTile = static_cast<EditorTileType>(file->default_type());
        for(int i = 0; i < _numOfTileY; ++ i)
        {
            for(int j = 0; j < _numOfTileX; ++ j)
            {
                if ( EditorTileType::DIRT == defaultTile ) _tileData[i][j] = "1_" + std::to_string(random(1, 3)) + "_1234";
                else if ( EditorTileType::GRASS == defaultTile ) _tileData[i][j] = "2_" + std::to_string(random(1, 3)) + "_1234";
                else if ( EditorTileType::WATER == defaultTile ) _tileData[i][j] = "3_" + std::to_string(random(1, 3)) + "_1234";
                else if ( EditorTileType::HILL == defaultTile ) _tileData[i][j] = "5_" + std::to_string(random(1, 3)) + "_1234";
            }
        }
        
        for ( auto tile = file->tiles()->begin(); tile != file->tiles()->end() ; ++ tile )
        {
            _tileData[tile->indices()->y()][tile->indices()->x()] =  tile->number()->str();
        }
  
        return true;
    }
    
    
    void GameMap::_parseFromFile(const char* fileName)
    {
        _data = FileUtils::getInstance()->getStringFromFile(fileName);
        _doc.Parse<0>(_data.c_str());
        
        _parseCollisionData();
        _parseItemData();
        _parseStartingLocationList();
        _parseTileData();
    }
    
    
    void GameMap::_parseCollisionData()
    {
        const rapidjson::Value& objs = _doc["Collisions"];
        for(SizeType i = 0; i < objs.Size(); ++ i)
        {
            Polygon p;
            const rapidjson::Value& vertices = objs[i];
            for(SizeType j = 0; j < vertices.Size() ; ++ j)
            {
                const rapidjson::Value& data = vertices[j];
                p.pushVertex(( Vec2(data["x"].GetDouble(), data["y"].GetDouble()) ));
            }
            _collisionData.push_back(p);
            
            // add clipped walls
            _gameMgr->getCellSpace()->addWall(p);
        }
    }
    
    
    void GameMap::_parseTileData()
    {
        const rapidjson::Value& tile = _doc["Tiles"];
        _numOfTileX = tile["width"].GetInt();
        _numOfTileY = tile["height"].GetInt();
        _tileData.resize(_numOfTileY);
        for(int i = 0 ; i < _numOfTileY; ++ i)
        {
            _tileData[i].resize(_numOfTileX);
        }
        const rapidjson::Value& tileNames = tile["type"];
        for(int i = 0; i < _numOfTileY; ++ i)
        {
            for(int j = 0; j < _numOfTileX; ++ j)
            {
                const rapidjson::Value& name = tileNames[j + i * _numOfTileX];
                _tileData[i][j] = name["name"].GetString();
            }
        }
    }
    
    
    void GameMap::_parseStartingLocationList()
    {
        const rapidjson::Value& locations = _doc["Locations"];
        for(SizeType i = 0 ; i < locations.Size() ; ++ i)
        {
            const rapidjson::Value& data = locations[i];
            _startingLocationList.push_back(Vec2(data["x"].GetInt(),data["y"].GetInt()));
        }
    }
    
    
    void GameMap::_parseItemData()
    {
        const rapidjson::Value& items = _doc["Items"];
        for(SizeType i = 0 ; i < items.Size() ; ++ i)
        {
            const rapidjson::Value& data = items[i];
            _itemData.push_back(NameCoordAmount(data["name"].GetString(),
                                                Vec2(data["x"].GetInt(), data["y"].GetInt()),
                                                data["amount"].GetInt()));
        }
    }
    
    
    std::pair<int,int> GameMap::getExactTileIndex(const Vec2& pos)
    {
        int centerTileIndexX = (int)((pos.x) / _sizeOfTile);        // not exact index!
        int centerTileIndexY = (int)((pos.y) / (_sizeOfTile / 2));  // not exact index!
        
        for(int i = centerTileIndexY - 2 ; i < centerTileIndexY + 2 ; ++ i)
        {
            for(int j = centerTileIndexX - 2 ; j < centerTileIndexX + 2 ; ++ j)
            {
                Vec2 pivot;
                if(i % 2 == 0)  pivot.x = (_sizeOfTile / 2) + (_sizeOfTile * j);
                else            pivot.x = _sizeOfTile * (j + 1);
                
                pivot.y = (_sizeOfTile / 2) * i;
                
                if(physics::isContainPointInDiamond(pivot, (_sizeOfTile / 2), pos))
                {
                    centerTileIndexX = j; // exact index!
                    centerTileIndexY = i; // exact index!
                    return std::make_pair(j, i);
                }
            }
        }
        return std::make_pair(0,0);
    }
    
    
    void GameMap::updateChunk(const Vec2& position)
    {
        log("<GameMap::updateChunk> re-load tiles at (%f, %f)", position.x, position.y);
        
        std::pair<int, int> originIndex = getExactTileIndex(position);
        int xIdx = 0;
        int yIdx = 0;
        for(int y = originIndex.second - _numOfViewableTileY / 2; y < originIndex.second + _numOfViewableTileY / 2 ; ++ y)
        {
            for(int x = originIndex.first - _numOfViewableTileX / 2; x < originIndex.first + _numOfViewableTileX / 2 ; ++ x)
            {
                if( xIdx == _numOfViewableTileX) xIdx = 0, yIdx ++;
                
                _currTiles[yIdx][xIdx]->setSpriteFrame(_tileData[y][x] + ".png");
                _currTiles[yIdx][xIdx]->setPosition(_getTilePositionFromIndex(x, y));
                
                xIdx++;
            }
        }
    }
    
    
    void GameMap::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags)
    {
        setPosition( getWorldPosition() - _gameMgr->getGameCamera()->getCameraPos() );
        Node::visit(renderer, transform, flags);
    }
    
    
    std::vector<Segment> GameMap::getNeighborWall(const cocos2d::Vec2 position) const
    {
        std::vector<Segment> ret;
        
        return ret;
    }
    
    
    GameMap::TileType GameMap::getStepOnTileType(const cocos2d::Vec2& pos)
    {
        std::pair<int, int> idx = getExactTileIndex(pos);
        std::string tile = _tileData[idx.second][idx.first];
        Vec2 center = _getTilePositionFromIndex(idx.first, idx.second);
        
        Vec2 region1 = center + Vec2(0.0f, _sizeOfTile / 4.0f);
        Vec2 region2 = center + Vec2(_sizeOfTile / 4.0f, 0.0f);
        Vec2 region3 = center - Vec2(0.0f, _sizeOfTile / 4.0f);
        Vec2 region4 = center - Vec2(_sizeOfTile / 4.0f, 0.0f);
        
        std::string tileType;
        for (unsigned k = 4; k < tile.size(); ++k)
            tileType += tile[k];
        
        if ( physics::isContainPointInDiamond(region1, _sizeOfTile / 4.0f, pos) && (tileType.find('1') != std::string::npos) )
        {
            return convertToTileType(tile.front());
        }
        else if ( physics::isContainPointInDiamond(region2, _sizeOfTile / 4.0f, pos) && (tileType.find('2') != std::string::npos) )
        {
            return convertToTileType(tile.front());
        }
        else if ( physics::isContainPointInDiamond(region3, _sizeOfTile / 4.0f, pos) && (tileType.find('3') != std::string::npos) )
        {
            return convertToTileType(tile.front());
        }
        else if ( physics::isContainPointInDiamond(region4, _sizeOfTile / 4.0f, pos) && (tileType.find('4') != std::string::npos) )
        {
            return convertToTileType(tile.front());
        }
        
        return TileType::DIRT;
    }
    
}












