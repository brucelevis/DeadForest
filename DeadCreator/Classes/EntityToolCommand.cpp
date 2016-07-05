//
//  EntityToolCommand.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "EntityToolCommand.hpp"
#include "GMXLayer2.hpp"
using namespace cocos2d;


void EntityToolCommand::execute()
{
    _layer->addEntity(_entity);
}


void EntityToolCommand::undo()
{
    _layer->eraseEntity(_entity);
}


EntityToolCommand* EntityToolCommand::clone() const
{
    return new EntityToolCommand(*this);
}


void EntityToolCommand::beginImpl()
{
    _entity = nullptr;
}


