//
//  AddEntityToolCommand.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "AddEntityToolCommand.hpp"
#include "GMXLayer.hpp"
using namespace realtrick;

AddEntityToolCommand::~AddEntityToolCommand()
{
    if ( _entity )
    {
        _layer->eraseEntity(_entity->getID());
    }
}


void AddEntityToolCommand::execute()
{
    _layer->addEntity(_entity, _entity->getLocalZOrder(), true);
}


void AddEntityToolCommand::undo()
{
    _layer->eraseEntity(_entity->getID(), true);
}


AddEntityToolCommand* AddEntityToolCommand::clone() const
{
    return new AddEntityToolCommand(*this);
}


void AddEntityToolCommand::beginImpl()
{
    _entity = nullptr;
}

void AddEntityToolCommand::endImpl()
{
    _entity = nullptr;
}









