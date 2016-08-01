//
//  CellSpacePartition.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 15..
//
//

#include "CellSpacePartition.hpp"
#include "GameObject.hpp"
#include "ClipperWrapper.hpp"
#include "GameResource.hpp"
using namespace realtrick::client;
using namespace realtrick;
using namespace cocos2d;


bool CellSpacePartition::initWithResource(GameResource* res)
{
    _worldWidth = res->getTileWidth() * res->getNumOfTileX();
    _worldHeight = res->getTileHeight() * res->getNumOfTileY();
    _cellWidth = res->getCellWidth();
    _cellHeight = res->getCellHeight();
    
    _numOfCellsX = (_worldWidth  / _cellWidth) + 2;
    _numOfCellsY = (_worldHeight / _cellHeight) + 2;
    
    for (int y = 0; y < _numOfCellsY; ++y)
    {
        for (int x = 0; x <_numOfCellsX; ++x)
        {
            float left  = x * _cellWidth;
            float bot   = y * _cellHeight;
            _cells.push_back(Cell(cocos2d::Rect(left - _cellWidth, bot - _cellHeight, _cellWidth, _cellHeight)));
        }
    }
    
    return true;
}


void CellSpacePartition::addEntity(GameObject* ent)
{
    int idx = positionToIndex(ent->getWorldPosition());
    _cells[idx].members.push_back(ent);
}


void CellSpacePartition::addWall(const Polygon& wall)
{
    //
    //      -------- -------- --------
    //     | 1      | 2      | 3      |
    //     |        |  ------|-. max  |                      _____       _
    //     |       -|-       | |      |            -        -     |     | |
    //     |      | |        | |      |        1: |_|    2:|______|  3: |_|
    //      -------- -------- --------             _                     _
    //     | 4    | | 5      |6|      |        4: |_|    5: ------   6: | |
    //     |       -|----    | |      |                    |      |     | |
    //     |        |    |   | |      |                     ---   |     | |
    //     |        |    |   | |      |                        |__|     |_|
    //      -------- -------- --------                          __       _
    //     | 7      | 8  |   |9|      |        7:        8:    |  |  9: | |
    //     |      . |     ---|-       |                         --       -
    //     |     min|        |        |
    //     |        |        |        |
    //      -------- -------- --------
    //
    //      해당 벽의 최대 최소 x, y를 각각 구해 AABB 를 만든다.
    //      공간들을 순회하면서 겹치는 공간이 있다면, 클리핑한다.
    //      그리고 해당 공간에 클리핑된 벽들을 각각 저장해 놓는다.
    
    float minx = std::min_element(std::begin(wall.vertices), std::end(wall.vertices), [](const cocos2d::Vec2& v1, const cocos2d::Vec2& v2) {
        return (v1.x < v2.x);
    })->x;
    
    float miny = std::min_element(std::begin(wall.vertices), std::end(wall.vertices), [](const cocos2d::Vec2& v1, const cocos2d::Vec2& v2) {
        return (v1.y < v2.y);
    })->y;
    
    float maxx = std::max_element(std::begin(wall.vertices), std::end(wall.vertices), [](const cocos2d::Vec2& v1, const cocos2d::Vec2& v2) {
        return (v1.x < v2.x);
    })->x;
    
    float maxy = std::max_element(std::begin(wall.vertices), std::end(wall.vertices), [](const cocos2d::Vec2& v1, const cocos2d::Vec2& v2) {
        return (v1.y < v2.y);
    })->y;
    
    cocos2d::Vec2 minVertex = cocos2d::Vec2(minx, miny);
    cocos2d::Vec2 maxVertex = cocos2d::Vec2(maxx, maxy);
    cocos2d::Rect wallAABB = cocos2d::Rect(minVertex, cocos2d::Size(maxVertex - minVertex));
    for( auto& cell : _cells )
    {
        cocos2d::Rect cellAABB = cell.boundingBox;
        if ( cellAABB.intersectsRect(wallAABB) )
        {
            std::vector<Polygon> clippedWalls = clipping::getClippedPolygons(wall, cellAABB);
            for( const auto& clippedWall : clippedWalls )
                cell.walls.push_back(clippedWall);
        }
    }
}


bool CellSpacePartition::updateEntity(GameObject* ent, cocos2d::Vec2 oldPos)
{
    int oldIdx = positionToIndex(oldPos);
    int newIdx = positionToIndex(ent->getWorldPosition());
    
    if (newIdx == oldIdx) return false;
    
    _cells[oldIdx].members.remove(ent);
    _cells[newIdx].members.push_back(ent);
    
    return true;
}


void CellSpacePartition::removeEntityFromCell(GameObject* ent)
{
    int index = positionToIndex(ent->getWorldPosition());
    auto iter = std::find(std::begin(_cells[index].members), std::end(_cells[index].members), ent);
    if(iter == std::end(_cells[index].members))
    {
        throw std::runtime_error("<CellSpacePartition::removeEntityFromCell> Cannot find entity in the cell.");
    }
    
    _cells[index].members.remove(ent);
}


void CellSpacePartition::clearCells()
{
    for (auto it = _cells.begin(); it != _cells.end(); ++it)
    {
        it->members.clear();
    }
}



int CellSpacePartition::positionToIndex(const cocos2d::Vec2& pos) const
{
    int idx = (int)( ( pos.x + _cellWidth ) / _cellWidth ) + (int)( ( pos.y + _cellHeight )  / _cellHeight ) * _numOfCellsX;
    CCASSERT(idx < (int)_cells.size(), "<CellSpacePartition::positionToIndex> index overflow (bad access)" );
    return idx;
}


const Cell& CellSpacePartition::getCell(const cocos2d::Vec2& pos) const
{
    int idx = positionToIndex(pos);
    return _cells[idx];
}


std::vector<int> CellSpacePartition::getNeighborCells(const cocos2d::Vec2& pos) const
{
    std::vector<int> ret(9);
    
    // 0 1 2
    // 3 4 5
    // 6 7 8
    int w = positionToIndex(pos);
    ret[0] = w + _numOfCellsX - 1;
    ret[1] = w + _numOfCellsX;
    ret[2] = w + _numOfCellsX + 1;
    ret[3] = w - 1;
    ret[4] = w;
    ret[5] = w + 1;
    ret[6] = w - _numOfCellsX - 1;
    ret[7] = w - _numOfCellsX;
    ret[8] = w - _numOfCellsX + 1;
    
    return ret;
}


std::vector<int> CellSpacePartition::getNeighborCellsNotCurrent(const cocos2d::Vec2& pos) const
{
    std::vector<int> ret(8);
    
    // 0 1 2
    // 3 x 4
    // 5 6 7
    int w = positionToIndex(pos);
    ret[0] = w + _numOfCellsX - 1;
    ret[1] = w + _numOfCellsX;
    ret[2] = w + _numOfCellsX + 1;
    ret[3] = w - 1;
    ret[4] = w + 1;
    ret[5] = w - _numOfCellsX - 1;
    ret[6] = w - _numOfCellsX;
    ret[7] = w - _numOfCellsX + 1;
    
    return ret;
}









