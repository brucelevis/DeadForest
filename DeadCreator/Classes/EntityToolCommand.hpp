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
        while ( !_entities.empty() )
        {
            auto remove = _entities.back();
            CC_SAFE_DELETE(remove);
            _entities.popBack();
        }
        
        _entities = rhs._entities;
    }
    
    virtual ~EntityToolCommand() = default;
    
    virtual void execute() override;
    
    virtual void undo() override;
    
    virtual EntityToolCommand* clone() const override;
    
    virtual bool empty() const override { return _entities.empty(); }
    
    void pushEntity(EntityBase* ent) { _entities.pushBack(ent); }
    
private:
    
    virtual void beginImpl() override;
    
private:
    
    
    cocos2d::Vector<EntityBase*> _entities;
    
};