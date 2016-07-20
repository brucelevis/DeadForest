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

namespace realtrick
{
    
    class CommandBase;
    
    class CommandQueue
    {
        
    public:
        
        CommandQueue() = default;
        
        virtual ~CommandQueue();
        
        void redo();
        void undo();
    
        int getIndex() const { return _currIndex; }
        
        bool isRedo() const;
        bool isUndo() const;
        
        int size() const { return static_cast<int>(_commands.size()); }
        void setStateToIndex(int index);
        
        const CommandBase* operator[](size_t index) const { return _commands[index]; }
        
        void pushCommand(CommandBase* command);
        
    private:
        
        std::vector<CommandBase*> _commands;
        int _currIndex = -1;
        
    };
    
}








