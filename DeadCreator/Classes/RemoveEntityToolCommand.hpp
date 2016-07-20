//
//  RemoveEntityToolCommand.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 5..
//
//

#pragma once

#include <vector>

#include "CommandBase.hpp"
#include "EditorEntity.hpp"

namespace realtrick
{
    
    class RemoveEntityToolCommand : public CommandBase
    {
        
    public:
        
        explicit RemoveEntityToolCommand(GMXLayer* layer) :
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
        void pushEntity(const std::vector<EditorEntity*>& entities) { if ( _isBegan ) _entities = entities; }
        
    private:
        
        virtual void beginImpl() override;
        virtual void endImpl() override;
        
    private:
        
        std::vector<EditorEntity*> _entities;
        
    };
    
}







