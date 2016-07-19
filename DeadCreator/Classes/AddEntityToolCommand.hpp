//
//  AddEntityToolCommand.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#pragma once

#include "CommandBase.hpp"
#include "EditorEntityBase.hpp"

namespace realtrick
{
    
    class AddEntityToolCommand : public CommandBase
    {
        
    public:
        
        explicit AddEntityToolCommand(GMXLayer* layer) :
        CommandBase(layer)
        {
            _commandName = "Add Entity";
        }
        
        AddEntityToolCommand(const AddEntityToolCommand& rhs) : CommandBase(rhs)
        {
            copyFrom(rhs);
        }
        
        void copyFrom(const AddEntityToolCommand& rhs)
        {
            _entity = rhs._entity;
        }
        
        virtual ~AddEntityToolCommand();
        
        virtual void execute() override;
        
        virtual void undo() override;
        
        virtual AddEntityToolCommand* clone() const override;
        
        virtual bool empty() const override { return !_entity; }
        
        void pushEntity(EditorEntityBase* ent) { if (_isBegan ) _entity = ent; }
        
    private:
        
        virtual void beginImpl() override;
        
        virtual void endImpl() override;
        
    private:
        
        EditorEntityBase* _entity = nullptr;
        
    };
    
}







