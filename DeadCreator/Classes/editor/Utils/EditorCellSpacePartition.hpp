//
//  EditorCellSpacePartition.hpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 15..
//
//

#pragma once

#include <vector>
#include <list>
#include <algorithm>

#include "cocos2d.h"

namespace realtrick
{
    
    class EditorEntity;
    
    struct  EditorCell
    {
        
        std::list<EditorEntity*> members;
        cocos2d::Rect boundingBox;
        
        EditorCell(cocos2d::Rect rect): boundingBox(rect) {}
        
    };
    
    
    class EditorCellSpacePartition : public cocos2d::Node
    {
        
    public:
        
        static EditorCellSpacePartition* create(const cocos2d::Size& worldSize, const cocos2d::Size& cellSize)
        {
            auto ret = new (std::nothrow) EditorCellSpacePartition();
            if ( ret && ret->init(worldSize, cellSize) )
            {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }
        
        bool init(const cocos2d::Size& worldSize, const cocos2d::Size& cellSize)
        {
            if ( !Node::init() )
                return false;
            
            _worldWidth = worldSize.width;
            _worldHeight = worldSize.height;
            
            _cellWidth = cellSize.width;
            _cellHeight = cellSize.height;
            
            _numOfCellsX = (worldSize.width  / cellSize.width) + 2;
            _numOfCellsY = (worldSize.height / cellSize.height) + 2;
            
            for (int y = 0; y < _numOfCellsY; ++y)
            {
                for (int x = 0; x <_numOfCellsX; ++x)
                {
                    float left  = x * cellSize.width;
                    float bot   = y * cellSize.height;
                    _cells.push_back(EditorCell(cocos2d::Rect(left - cellSize.width, bot - cellSize.height, cellSize.width, cellSize.height)));
                }
            }
            
            return true;
        }
        
        EditorCellSpacePartition() = default;
        
        inline void                     addEntity(EditorEntity* ent);
        
        inline bool                     isUpdateChunk(const cocos2d::Vec2& oldPos, const cocos2d::Vec2& newPos);
        
        inline void                     calculateNeighbors(const cocos2d::Vec2& targetPos, float queryRadius);
        
        inline void                     clearCells();
        inline void                     removeEntityFromCell(EditorEntity* ent);
        
        inline int                      positionToIndex(const cocos2d::Vec2& pos) const;
        
        inline const EditorCell&        getCell(const cocos2d::Vec2& pos) const;
        inline const EditorCell&        getCell(int index)  { return _cells[index]; }
        
        inline float                    getWorldWidth() const { return _worldWidth; }
        inline float                    getWorldHeight() const { return _worldHeight; }
        
        inline int                      getNumOfCellX() const { return _numOfCellsX; }
        inline int                      getNumOfCellY() const { return _numOfCellsY; }
        
        inline float                    getCellWidth() const { return _cellWidth; }
        inline float                    getCellHeight() const { return _cellHeight; }
        
        inline std::vector<int>         getNeighborCells(const cocos2d::Vec2& pos) const;
        inline std::vector<int>         getNeighborCellsNotCurrent(const cocos2d::Vec2& pos) const;
        
        inline const std::vector<EditorCell>& getCells() const { return _cells; }
        
    private:
        
        std::vector<EditorCell> _cells;
        
        float                   _worldWidth;
        float                   _worldHeight;
        
        int                     _numOfCellsX;
        int                     _numOfCellsY;
        
        float                   _cellWidth;
        float                   _cellHeight;
        
    };
    
}

#include "EditorCellSpacePartition.inl"









