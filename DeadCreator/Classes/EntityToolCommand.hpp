//
//  EntityToolCommand.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#pragma once

#include <vector>

#include "CommandBase.hpp"
#include "EntityBase.hpp"

class EntityToolCommand : public CommandBase
{
    
public:
    
    explicit EntityToolCommand(GMXLayer2* layer) :
    CommandBase(layer)
    {
        _commandName = "Entity Tool";
    }
    
    EntityToolCommand(const EntityToolCommand& rhs) : CommandBase(rhs)
    {
        copyFrom(rhs);
    }
    
    void copyFrom(const EntityToolCommand& rhs)
    {
        _entity = rhs._entity;
    }
    
    virtual ~EntityToolCommand() = default;
    
    virtual void execute() override;
    
    virtual void undo() override;
    
    virtual EntityToolCommand* clone() const override;
    
    virtual bool empty() const override { return !_entity; }
    
    void pushEntity(EntityBase* ent) { _entity = ent; _entity->retain(); }
    
private:
    
    virtual void beginImpl() override;
    
private:
    
    EntityBase* _entity;
    
};