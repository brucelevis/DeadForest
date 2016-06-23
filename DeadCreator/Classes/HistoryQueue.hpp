//
//  HistoryQueue.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 23..
//
//

#ifndef HistoryQueue_hpp
#define HistoryQueue_hpp

#include <vector>

#include "cocos2d.h"

class HistoryBase;

class HistoryQueue : public cocos2d::Node
{
  
public:
    
    CREATE_FUNC(HistoryQueue);
    
    void redo();
    
    void undo();
    
    void push(HistoryBase* history);
    
    int getIndex() const { return _currIndex; }
    
    bool isRedo() const;
    
    bool isUndo() const;
    
private:
    
    std::vector<HistoryBase*> _historys;
    
    int _currIndex = -1;
    
};

#endif /* HistoryQueue_hpp */









