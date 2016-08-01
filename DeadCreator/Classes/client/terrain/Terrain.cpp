//
//  Terrain.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#include "Terrain.hpp"
#include "RenderingSystem.hpp"
#include "GameResource.hpp"
#include "TileHelperFunctions.hpp"
#include "TileImage.hpp"
using namespace realtrick::client;


Terrain::Terrain(RenderingSystem* renderSys) :
_renderSys(renderSys),
_worldPosition(cocos2d::Vec2::ZERO),
_numOfViewableTileX(0),
_numOfViewableTileY(0)
{}


Terrain::~Terrain()
{}


Terrain* Terrain::create(RenderingSystem* renderSys, GameResource* res)
{
    auto ret = new (std::nothrow) Terrain(renderSys);
    if ( ret && ret->init(res) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool Terrain::init(GameResource* res)
{
    if ( !Node::init() )
        return false;
    
    _gameResource = res;
    
    _numOfViewableTileX = Prm.getValueAsInt("numOfViewableTileX");
    _numOfViewableTileY = Prm.getValueAsInt("numOfViewableTileY");
    
    _currTiles.resize(_numOfViewableTileY);
    for (int i = 0; i < _numOfViewableTileY; ++i)
    {
        _currTiles[i].resize(_numOfViewableTileX);
    }
    
    for (int i = 0; i < _numOfViewableTileY; ++i)
    {
        for (int j = 0; j < _numOfViewableTileX; ++j)
        {
            _currTiles[i][j] = realtrick::TileImage::create();
            addChild(_currTiles[i][j] , 1);
        }
    }
    
    return true;
}


void Terrain::updateChunk(const cocos2d::Vec2& position)
{
    int tileWidth = _gameResource->getTileWidth();
    int tileHeight = _gameResource->getTileHeight();
    const auto& tileData = _gameResource->getTileData();
    
    std::pair<int, int> originIndex = getFocusedTileIndex(position, tileWidth, tileHeight, DUMMY_TILE_SIZE);
    int xIdx = 0;
    int yIdx = 0;
    for(int y = originIndex.second - _numOfViewableTileY / 2; y < originIndex.second + _numOfViewableTileY / 2 ; ++ y)
    {
        for(int x = originIndex.first - _numOfViewableTileX / 2; x < originIndex.first + _numOfViewableTileX / 2 ; ++ x)
        {
            if ( x < 0 || y < 0 || y >= _gameResource->getNumOfTileY() || x >= _gameResource->getNumOfTileX() ) continue;
            if( xIdx == _numOfViewableTileX) xIdx = 0, yIdx ++;
            
            _currTiles[yIdx][xIdx]->setSpriteFrame(tileData[y][x].getNumber() + ".png");
            _currTiles[yIdx][xIdx]->setPosition(tileData[y][x].getPosition());
            
            xIdx++;
        }
    }
}


void Terrain::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
    setPosition( _worldPosition - _renderSys->getCameraPosition() );
    Node::visit(renderer, transform, flags);
}









