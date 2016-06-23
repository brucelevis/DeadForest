//
//  HistoryQueue.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 23..
//
//

#include "HistoryQueue.hpp"
#include "HistoryBase.hpp"


void HistoryQueue::redo()
{
    _historys[_currIndex + 1]->redo();
    _currIndex ++;
}


void HistoryQueue::undo()
{
    _historys[_currIndex]->undo();
    _currIndex--;
}


void HistoryQueue::push(HistoryBase* history)
{
    while( _historys.size() > _currIndex + 1 )
    {
        auto remove = _historys.back();
        delete remove;
        remove = nullptr;
        _historys.pop_back();
    }
    
    _historys.push_back(history);
    _currIndex ++;
}


bool HistoryQueue::isRedo() const
{
    return (_currIndex < _historys.size() - 1);
}


bool HistoryQueue::isUndo() const
{
    return (_currIndex > -1);
}









