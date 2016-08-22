//
//  CellSpacePartition.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 15..
//
//

#pragma once

#include <vector>
#include <list>
#include <algorithm>

#include "cocos2d.h"
#include "Physics.hpp"

namespace realtrick
{
    
    namespace client
    {
        
        class EntityBase;
        class GameResource;
        
        struct Cell
        {
            
            std::list<EntityBase*>                      members;
            std::vector<realtrick::Polygon>             walls;
            cocos2d::Rect                               boundingBox;
            
            Cell(cocos2d::Rect rect): boundingBox(rect) {}
            
        };
        
        
        class CellSpacePartition : public cocos2d::Ref
        {
            
        public:
            
            static CellSpacePartition* createWithResource(GameResource* res)
            {
                auto ret = new (std::nothrow) CellSpacePartition();
                if ( ret && ret->initWithResource(res) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool initWithResource(GameResource* res);
            
            void                     addEntity(EntityBase* ent);
            void                     addWall(const realtrick::Polygon& wall);
            
            bool                     updateEntity(EntityBase* ent, cocos2d::Vec2 oldPos);
            
            void                     removeEntityFromCell(EntityBase* ent);
            int                      positionToIndex(const cocos2d::Vec2& pos) const;
            
            const Cell&              getCell(const cocos2d::Vec2& pos) const;
            const Cell&              getCell(int index)  { return _cells[index]; }
            const std::vector<Cell>& getCells() const { return _cells; }
            void                     clearCells();
            
            float                    getWorldWidth() const { return _worldWidth; }
            float                    getWorldHeight() const { return _worldHeight; }
            int                      getNumOfCellX() const { return _numOfCellsX; }
            int                      getNumOfCellY() const { return _numOfCellsY; }
            float                    getCellWidth() const { return _cellWidth; }
            float                    getCellHeight() const { return _cellHeight; }
            
            std::vector<int>         getNeighborCells(const cocos2d::Vec2& pos) const;
            std::vector<int>         getNeighborCellsNotCurrent(const cocos2d::Vec2& pos) const;
            
        private:
            
            CellSpacePartition() = default;
            
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
}









