//
//  TileToolCommand.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 3..
//
//

#include "TileToolCommand.hpp"
#include "GMXLayer.hpp"
using namespace realtrick;

void TileToolCommand::execute()
{
    for( auto tile = _currTiles.cbegin() ; tile != _currTiles.cend() ; ++ tile)
    {
        _layer->setTile(tile->getIndexX(), tile->getIndexY(), *tile, true);
    }
}


void TileToolCommand::undo()
{
    for( auto tile = _prevTiles.crbegin() ; tile != _prevTiles.crend() ; ++ tile)
    {
        _layer->setTile(tile->getIndexX(), tile->getIndexY(), *tile, true);
    }
}


void TileToolCommand::pushTile(const TileBase& prevTile, const TileBase& currTile)
{
    if ( _isBegan )
    {
        _prevTiles.push_back(prevTile);
        _currTiles.push_back(currTile);
    }
}


TileToolCommand* TileToolCommand::clone() const
{
    return new TileToolCommand(*this);
}


void TileToolCommand::beginImpl()
{
    _prevTiles.clear();
    _currTiles.clear();
}







