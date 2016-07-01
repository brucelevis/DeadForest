//
//  MutableUiBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 28..
//
//

#ifndef MutableUiBase_hpp
#define MutableUiBase_hpp

#include <vector>

#include "TileBase.hpp"

class MutableUiBase
{
    
public:
    
    virtual void setTile(int x, int y, const TileBase& tile)
    {
        _tiles[y][x] = tile;
    }
    
protected:
    
    std::vector<std::vector<TileBase>> _tiles;
    
};

#endif /* MutableUiBase_hpp */
