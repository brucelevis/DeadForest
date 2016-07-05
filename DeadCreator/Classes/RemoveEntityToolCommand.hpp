//
//  RemoveEntityToolCommand.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 5..
//
//

#include <vector>

#include "CommandBase.hpp"
#include "EntityBase.hpp"

#pragma once

class RemoveEntityToolCommand : public CommandBase
{
    
public:
    
    explicit RemoveEntityToolCommand(GMXLayer2* layer) :
    CommandBase(layer)
    {
        _commandName = "Remove Entity";
    }
    
    RemoveEntityToolCommand(const RemoveEntityToolCommand& rhs) : CommandBase(rhs)
    {
        copyFrom(rhs);
    }
    
    void copyFrom(const RemoveEntityToolCommand& rhs)
    {
        _entities = rhs._entities;
    }
    
    virtual ~RemoveEntityToolCommand();
    
    virtual void execute() override;
    
    virtual void undo() override;
    
    virtual RemoveEntityToolCommand* clone() const override;
    
    virtual bool empty() const override { return _entities.empty(); }
    
    void pushEntity(const std::vector<EntityBase*>& entities) { if ( _isBegan ) _entities = entities; }
    
private:
    
    virtual void beginImpl() override;
    
    virtual void endImpl() override;
    
private:
    
    std::vector<EntityBase*> _entities;
    
};









