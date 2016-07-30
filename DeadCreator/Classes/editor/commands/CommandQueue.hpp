//
//  CommandQueue.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 3..
//
//

#pragma once

#include <vector>

#include "cocos2d.h"

#include "CommandBase.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class CommandBase;
        
        class CommandQueue
        {
            
        public:
            
            CommandQueue() = default;
            virtual ~CommandQueue()
            {
                for ( auto& command : _commands ) CC_SAFE_DELETE(command);
                _commands.clear();
            }
            
            void redo()
            {
                _commands[_currIndex + 1]->execute();
                _currIndex ++;
            }
            
            void undo()
            {
                _commands[_currIndex]->undo();
                _currIndex--;
            }
            
            int getIndex() const { return _currIndex; }
            bool isRedo() const { return (_currIndex + 1 < _commands.size()); }
            bool isUndo() const { return (_currIndex > -1); }
            int size() const { return static_cast<int>(_commands.size()); }
            void setStateToIndex(int index)
            {
                if ( _currIndex > index ) while( _currIndex != index ) { undo(); }
                else while( _currIndex != index ) { redo(); }
            }
            
            const CommandBase* operator[](size_t index) const { return _commands[index]; }
            void pushCommand(CommandBase* command)
            {
                while( _commands.size() > _currIndex + 1 )
                {
                    auto remove = _commands.back();
                    delete remove;
                    remove = nullptr;
                    _commands.pop_back();
                }
                
                _commands.push_back(command);
                _currIndex ++;
            }
            
        private:
            
            std::vector<CommandBase*> _commands;
            int _currIndex = -1;
            
        };
        
    }
}









