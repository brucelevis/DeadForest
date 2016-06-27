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

class TerrainBase;

class MutableUiBase
{
    
public:
    
    virtual void setTerrain(int x, int y, TerrainBase* terrain) { _tiles[y][x] = terrain; }
    
protected:
    
    std::vector<std::vector<TerrainBase*>> _tiles;
    
};

#endif /* MutableUiBase_hpp */
