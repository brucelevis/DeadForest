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
#include "TileHelperFunctions.hpp"
using namespace realtrick;
using namespace realtrick::client;
using namespace cocos2d;


GameMap::GameMap(GameManager* gameMgr) : _gameMgr(gameMgr),
_numOfTileX(0),
_numOfTileY(0),
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
    _tileWidth              = file->tile_size()->width();
    _tileHeight             = file->tile_size()->height();
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
    
    auto defaultTile = static_cast<TileType>(file->default_type());
    for(int i = 0; i < _numOfTileY; ++ i)
    {
        for(int j = 0; j < _numOfTileX; ++ j)
        {
            auto pos = indexToPosition(j, i, file->tile_size()->width(), file->tile_size()->height(), DUMMY_TILE_SIZE);
            
            if ( TileType::DIRT == defaultTile ) _tileData[i][j]= TileBase(j, i,"1_" + _to_string(random(1, 3)) + "_1234", pos);
            else if ( TileType::GRASS == defaultTile ) _tileData[i][j]= TileBase(j, i,"2_" + _to_string(random(1, 3)) + "_1234", pos);
            else if ( TileType::WATER == defaultTile ) _tileData[i][j]= TileBase(j, i,"3_" + _to_string(random(1, 3)) + "_1234", pos);
            else if ( TileType::HILL == defaultTile ) _tileData[i][j]= TileBase(j, i,"5_" + _to_string(random(1, 3)) + "_1234", pos);
        }
    }
    
    for ( auto tile = file->tiles()->begin(); tile != file->tiles()->end() ; ++ tile )
    {
        _tileData[tile->indices()->y()][tile->indices()->x()].setNumber(tile->number()->str());
    }
    
    return true;
}


void GameMap::updateChunk(const Vec2& position)
{
    log("<GameMap::updateChunk> re-load tiles at (%f, %f)", position.x, position.y);
    
    std::pair<int, int> originIndex = getFocusedTileIndex(position, _tileWidth, _tileHeight, DUMMY_TILE_SIZE);
    int xIdx = 0;
    int yIdx = 0;
    for(int y = originIndex.second - _numOfViewableTileY / 2; y < originIndex.second + _numOfViewableTileY / 2 ; ++ y)
    {
        for(int x = originIndex.first - _numOfViewableTileX / 2; x < originIndex.first + _numOfViewableTileX / 2 ; ++ x)
        {
            if ( x < 0 || y < 0 || y >= _numOfTileY || x >= _numOfTileX ) continue;
            if( xIdx == _numOfViewableTileX) xIdx = 0, yIdx ++;
            
            _currTiles[yIdx][xIdx]->setSpriteFrame(_tileData[y][x].getNumber() + ".png");
            _currTiles[yIdx][xIdx]->setPosition(_tileData[y][x].getPosition());
            
            xIdx++;
        }
    }
}


void GameMap::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
    setPosition( getWorldPosition() - _gameMgr->getGameCamera()->getCameraPos() );
    Node::visit(renderer, transform, flags);
}


TileType GameMap::getStepOnTileType(const cocos2d::Vec2& pos)
{
    std::pair<int, int> idx = getFocusedTileIndex(pos, _tileWidth, _tileHeight, DUMMY_TILE_SIZE);
    TileBase& tile = _tileData[idx.second][idx.first];
    Vec2 center = indexToPosition(idx.first, idx.second, _tileWidth, _tileHeight, DUMMY_TILE_SIZE);
    
    Vec2 region1 = center + Vec2(0.0f, _tileHeight / 4.0f);
    Vec2 region2 = center + Vec2(_tileWidth / 4.0f, 0.0f);
    Vec2 region3 = center - Vec2(0.0f, _tileHeight / 4.0f);
    Vec2 region4 = center - Vec2(_tileWidth / 4.0f, 0.0f);
    
    std::string tileType = tile.getTileTail();
    
    if ( physics::isContainPointInDiamond(region1, _tileWidth / 4.0f, pos) && (tileType.find('1') != std::string::npos) )
    {
        return tile.getTileType();
    }
    else if ( physics::isContainPointInDiamond(region2, _tileWidth / 4.0f, pos) && (tileType.find('2') != std::string::npos) )
    {
        return tile.getTileType();
    }
    else if ( physics::isContainPointInDiamond(region3, _tileWidth / 4.0f, pos) && (tileType.find('3') != std::string::npos) )
    {
        return tile.getTileType();
    }
    else if ( physics::isContainPointInDiamond(region4, _tileWidth / 4.0f, pos) && (tileType.find('4') != std::string::npos) )
    {
        return tile.getTileType();
    }
    
    return TileType::DIRT;
}










