//
//  EditorCellSpacePartition.inl
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 15..
//
//

#include "EditorCellSpacePartition.hpp"
#include "EditorEntity.hpp"

namespace realtrick
{
    
    inline void EditorCellSpacePartition::addEntity(EditorEntity* ent)
    {
        int idx = positionToIndex(ent->getPosition());
        _cells[idx].members.push_back(ent);
    }
    
    
    inline bool EditorCellSpacePartition::isUpdateChunk(const cocos2d::Vec2& oldPos, const cocos2d::Vec2& newPos)
    {
        int oldIdx = positionToIndex(oldPos);
        int newIdx = positionToIndex(newPos);
        
        if ( newIdx == oldIdx ) return false;
        
        return true;
    }
    
    
    inline void EditorCellSpacePartition::removeEntityFromCell(EditorEntity* ent)
    {
        int index = positionToIndex(ent->getPosition());
        auto iter = std::find(std::begin(_cells[index].members), std::end(_cells[index].members), ent);
        if(iter == std::end(_cells[index].members))
        {
            throw std::runtime_error("<EditorCellSpacePartition::removeEntityFromCell> Cannot find entity in the cell.");
        }
        
        _cells[index].members.remove(ent);
    }
    
    
    inline void EditorCellSpacePartition::clearCells()
    {
        for (auto it = _cells.begin(); it != _cells.end(); ++it)
        {
            it->members.clear();
        }
    }
    
    
    
    inline int EditorCellSpacePartition::positionToIndex(const cocos2d::Vec2& pos) const
    {
        int idx = (int)( ( pos.x + _cellWidth ) / _cellWidth ) + (int)( ( pos.y + _cellHeight )  / _cellHeight ) * _numOfCellsX;
        CCASSERT(idx < (int)_cells.size(), "<EditorCellSpacePartition::positionToIndex> index overflow (bad access)" );
        return idx;
    }
    
    
    inline const EditorCell& EditorCellSpacePartition::getCell(const cocos2d::Vec2& pos) const
    {
        int idx = positionToIndex(pos);
        return _cells[idx];
    }
    
    
    inline std::vector<int> EditorCellSpacePartition::getNeighborCells(const cocos2d::Vec2& pos) const
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
    
    
    inline std::vector<int> EditorCellSpacePartition::getNeighborCellsNotCurrent(const cocos2d::Vec2& pos) const
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










