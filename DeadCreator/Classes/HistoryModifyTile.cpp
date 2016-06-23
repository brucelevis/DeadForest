//
//  HistoryModifyTile.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 23..
//
//

#include "HistoryModifyTile.hpp"
#include "Telegram.hpp"


void HistoryModifyTile::redo()
{
    auto receiver = Dispatch.getNodePtr(MessageNodeType::GMX_LAYER);
    if ( receiver )
    {
        Dispatch.pushMessage(0.0, receiver, this, MessageType::REDO_TILE, &modifiedDatas);
    }
    else
    {
        cocos2d::log("<HistoryModifyTile::redo> receiver is not exist.");
    }
}


void HistoryModifyTile::undo()
{
    auto receiver = Dispatch.getNodePtr(MessageNodeType::GMX_LAYER);
    if ( receiver)
    {
        Dispatch.pushMessage(0.0, receiver, this, MessageType::UNDO_TILE, &modifiedDatas);
    }
    else
    {
        cocos2d::log("<HistoryModifyTile::undo> receiver is not exist.");
    }
}










