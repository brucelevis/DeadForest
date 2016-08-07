
#include "CellSpacePartition.hpp"
#include "EntityBase.hpp"
#include "ClipperWrapper.hpp"



namespace realtrick
{
    
    inline void CellSpacePartition::addEntity(EntityBase* ent)
    {
        int idx = positionToIndex(ent->getWorldPosition());
        _cells[idx].members.push_back(ent);
    }
    
    
    
    inline void CellSpacePartition::addWall(const geometry::Polygon& wall)
    {
        float minx = std::min_element(std::begin(wall.vertices), std::end(wall.vertices), [](const Vec2& v1, const Vec2& v2) {
            return (v1.x < v2.x);
        })->x;
        
        float miny = std::min_element(std::begin(wall.vertices), std::end(wall.vertices), [](const Vec2& v1, const Vec2& v2) {
            return (v1.y < v2.y);
        })->y;
        
        float maxx = std::max_element(std::begin(wall.vertices), std::end(wall.vertices), [](const Vec2& v1, const Vec2& v2) {
            return (v1.x < v2.x);
        })->x;
        
        float maxy = std::max_element(std::begin(wall.vertices), std::end(wall.vertices), [](const Vec2& v1, const Vec2& v2) {
            return (v1.y < v2.y);
        })->y;
        
        Vec2 minVertex = Vec2(minx, miny);
        Vec2 maxVertex = Vec2(maxx, maxy);
        Rect wallAABB = Rect(minVertex, Size(maxVertex - minVertex));
        for( auto& cell : _cells )
        {
            Rect cellAABB = cell.boundingBox;
            if ( cellAABB.intersectsRect(wallAABB) )
            {
                std::vector<geometry::Polygon> clippedWalls = clipping::getClippedPolygons(wall, cellAABB);
                for( const auto& clippedWall : clippedWalls )
                    cell.walls.push_back(clippedWall);
            }
        }
    }
    
    
    inline bool CellSpacePartition::updateEntity(EntityBase* ent, Vec2 oldPos)
    {
        int oldIdx = positionToIndex(oldPos);
        int newIdx = positionToIndex(ent->getWorldPosition());
        
        if (newIdx == oldIdx) return false;
        
        _cells[oldIdx].members.remove(ent);
        _cells[newIdx].members.push_back(ent);
        
        return true;
    }
    
    
    inline void CellSpacePartition::removeEntityFromCell(EntityBase* ent)
    {
        int index = positionToIndex(ent->getWorldPosition());
        auto iter = std::find(std::begin(_cells[index].members), std::end(_cells[index].members), ent);
        if(iter == std::end(_cells[index].members))
        {
            throw std::runtime_error("<CellSpacePartition::removeEntityFromCell> Cannot find entity in the cell.");
        }
        
        _cells[index].members.remove(ent);
    }
    
    
    inline void CellSpacePartition::clearCells()
    {
        for (auto it = _cells.begin(); it != _cells.end(); ++it)
        {
            it->members.clear();
        }
    }
    
    
    
    inline int CellSpacePartition::positionToIndex(const Vec2& pos) const
    {
        int idx = (int)( ( pos.x + _cellWidth ) / _cellWidth ) + (int)( ( pos.y + _cellHeight )  / _cellHeight ) * _numOfCellsX;
        return idx;
    }
    
    
    inline const Cell& CellSpacePartition::getCell(const Vec2& pos) const
    {
        int idx = positionToIndex(pos);
        return _cells[idx];
    }
    
    
    inline std::vector<int> CellSpacePartition::getNeighborCells(const Vec2& pos) const
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
        
        return std::move(ret);
    }
    
    
    inline std::vector<int> CellSpacePartition::getNeighborCellsNotCurrent(const Vec2& pos) const
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
        
        return std::move(ret);
    }
    
}











