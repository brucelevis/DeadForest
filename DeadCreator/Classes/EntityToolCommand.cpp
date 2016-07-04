//
//  EntityToolCommand.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "EntityToolCommand.hpp"
#include "GMXLayer2.hpp"

void EntityToolCommand::execute()
{
}


void EntityToolCommand::undo()
{
}


EntityToolCommand* EntityToolCommand::clone() const
{
    return new EntityToolCommand(*this);
}


void EntityToolCommand::beginImpl()
{
}


