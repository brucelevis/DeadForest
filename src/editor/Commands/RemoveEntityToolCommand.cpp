//
//  RemoveEntityToolCommand.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 5..
//
//

#include "RemoveEntityToolCommand.hpp"
#include "GMXLayer.hpp"
using namespace realtrick;

RemoveEntityToolCommand::~RemoveEntityToolCommand()
{
    _entities.clear();
    cocos2d::log("~RemoveEntityToolCommand");
}


void RemoveEntityToolCommand::execute()
{
    for( auto& ent : _entities )
    {
        _layer->eraseEntity(ent->getID(), true);
    }
}


void RemoveEntityToolCommand::undo()
{
    for( auto& ent : _entities )
    {
        _layer->addEntity(ent, ent->getLocalZOrder(), true);
    }
}


RemoveEntityToolCommand* RemoveEntityToolCommand::clone() const
{
    return new RemoveEntityToolCommand(*this);
}


void RemoveEntityToolCommand::beginImpl()
{
    _entities.clear();
}


void RemoveEntityToolCommand::endImpl()
{
    _entities.clear();
}









