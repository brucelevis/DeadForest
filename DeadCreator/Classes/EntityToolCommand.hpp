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
        
    }
    
    virtual ~EntityToolCommand() = default;
    
    virtual void execute() override;
    
    virtual void undo() override;
    
    virtual EntityToolCommand* clone() const override;
    
    bool empty() const { return false; }
    
private:
    
    virtual void beginImpl() override;
    
private:
    
    bool _isEmpty;
    
};