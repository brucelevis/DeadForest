//
//  CommandQueue.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 3..
//
//

#pragma once

#include <vector>

#include "cocos2d.h"

class CommandBase;

class CommandQueue
{

public:

    CommandQueue() = default;
    
    virtual ~CommandQueue();

    void redo();

    void undo();

    void pushCommand(CommandBase* command);

    int getIndex() const { return _currIndex; }

    bool isRedo() const;

    bool isUndo() const;
    
    int size() const { return static_cast<int>(_commands.size()); }

private:

    std::vector<CommandBase*> _commands;

    int _currIndex = -1;

};









