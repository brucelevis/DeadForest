#pragma once

#include <vector>
#include <list>
#include <algorithm>

#include "Rect.h"
#include "Vec2.h"
#include "Physics.hpp"


namespace realtrick
{
    
    class EntityBase;
    
    class Cell
    {
        
    public:
        
        std::list<EntityBase*>                      members;
        
        std::vector<geometry::Polygon>              walls;
        
        Rect										boundingBox;
        
        Cell(Rect rect): boundingBox(rect)
        {}
        
    };
    
    
    class CellSpacePartition
    {
        
    public:
        
        CellSpacePartition() = delete;
        
        explicit CellSpacePartition(float worldWidth, float worldHeight, int cellWidth, int cellHeight, int maxEntities) :
        _worldWidth(worldWidth),
        _worldHeight(worldHeight),
        _cellWidth(cellWidth),
        _cellHeight(cellHeight)
        {
            _numOfCellsX = (worldWidth  / cellWidth) + 2;
            _numOfCellsY = (worldHeight / cellHeight) + 2;
            
            for (int y = 0; y < _numOfCellsY; ++y)
            {
                for (int x = 0; x <_numOfCellsX; ++x)
                {
                    float left  = x * cellWidth;
                    float bot   = y * cellHeight;
                    _cells.push_back(Cell(Rect(left - _cellWidth, bot - _cellHeight, cellWidth, cellHeight)));
                }
            }
            
        }
    
        inline void                     addEntity(EntityBase* ent);
        
        inline void                     addWall(const geometry::Polygon& wall);
        
        inline bool                     updateEntity(EntityBase* ent, Vec2 oldPos);
        
        inline void                     calculateNeighbors(Vec2 targetPos, float queryRadius);
        
        inline void                     clearCells();
        
        inline void                     removeEntityFromCell(EntityBase* ent);
        
        inline int                      positionToIndex(const Vec2& pos) const;
        
        inline const Cell&              getCell(const Vec2& pos) const;
        
        inline const Cell&              getCell(int index)  { return _cells[index]; }
        
        inline float                    getWorldWidth() const { return _worldWidth; }
        
        inline float                    getWorldHeight() const { return _worldHeight; }
        
        inline int                      getNumOfCellX() const { return _numOfCellsX; }
        
        inline int                      getNumOfCellY() const { return _numOfCellsY; }
        
        inline float                    getCellWidth() const { return _cellWidth; }
        
        inline float                    getCellHeight() const { return _cellHeight; }
        
        inline std::vector<int>         getNeighborCells(const Vec2& pos) const;
        
        inline std::vector<int>         getNeighborCellsNotCurrent(const Vec2& pos) const;
        
        inline const std::vector<Cell>& getCells() const { return _cells; }
        
    private:
        
        std::vector<Cell>       _cells;
        
        float                   _worldWidth;
        
        float                   _worldHeight;
        
        int                     _numOfCellsX;
        
        int                     _numOfCellsY;
        
        float                   _cellWidth;
        
        float                   _cellHeight;
    
    };
    
}

#include "CellSpacePartition.inl"





