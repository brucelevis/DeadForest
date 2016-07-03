//
//  CommandQueue.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 3..
//
//

#include "CommandQueue.hpp"
#include "CommandBase.hpp"

CommandQueue::~CommandQueue()
{
    while ( !_commands.empty() )
    {
        auto remove = _commands.back();
        delete remove;
        remove = nullptr;
        _commands.pop_back();
    }
}


void CommandQueue::redo()
{
    _commands[_currIndex + 1]->execute();
    _currIndex ++;
}


void CommandQueue::undo()
{
    _commands[_currIndex]->undo();
    _currIndex--;
}


void CommandQueue::pushCommand(CommandBase* command)
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


void CommandQueue::setStateToIndex(int index)
{
    if ( _currIndex > index )
    {
        while( _currIndex != index )
        {
            undo();
        }
    }
    else
    {
        while( _currIndex != index )
        {
            redo();
        }
    }
}


bool CommandQueue::isRedo() const
{
    return (_currIndex + 1 < _commands.size());
}


bool CommandQueue::isUndo() const
{
    return (_currIndex > -1);
}









