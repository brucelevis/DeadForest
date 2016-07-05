//
//  AddEntityToolCommand.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "AddEntityToolCommand.hpp"
#include "GMXLayer2.hpp"
using namespace cocos2d;


AddEntityToolCommand::~AddEntityToolCommand()
{
    if ( _entity )
    {
        _layer->eraseEntity(_entity->getID());
    }
}


void AddEntityToolCommand::execute()
{
    _entity->setVisible(true);
    _layer->addEntity(_entity, _entity->getLocalZOrder(), true);
}


void AddEntityToolCommand::undo()
{
    _entity->setVisible(false);
    _entity->setSelected(false);
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









