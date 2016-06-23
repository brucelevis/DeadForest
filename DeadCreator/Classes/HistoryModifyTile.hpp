//
//  HistoryModifyTile.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 23..
//
//

#ifndef HistoryModifyTile_hpp
#define HistoryModifyTile_hpp

#include "HistoryBase.hpp"
#include "Tiling.hpp"


struct HistoryModifyTile : public HistoryBase
{
    
    std::vector<ModifiedData> modifiedDatas;
    
    HistoryModifyTile() = default;
    
    virtual ~HistoryModifyTile() = default;
    
    virtual void redo() override;
    
    virtual void undo() override;
    
};

#endif /* HistoryModifyTile_hpp */










