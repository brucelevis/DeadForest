//
//  AddEntityToolCommand.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#pragma once

#include "CommandBase.hpp"
#include "EditorEntity.hpp"
#include "GMXLayer.hpp"

namespace realtrick
{
    namespace editor
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
            
            virtual ~AddEntityToolCommand() { if ( _entity ) _layer->eraseEntity(_entity->getID()); }
            
            virtual void execute() override
            {
                _layer->addEntity(_entity, _entity->getLocalZOrder(), true);
            }
            
            virtual void undo() override
            {
                _layer->eraseEntity(_entity->getID(), true);
            }
            
            virtual AddEntityToolCommand* clone() const override { return new AddEntityToolCommand(*this); }
            virtual bool empty() const override { return !_entity; }
            void pushEntity(EditorEntity* ent) { if (_isBegan ) _entity = ent; }
            
        private:
            
            virtual void beginImpl() override { _entity = nullptr; }
            virtual void endImpl() override { _entity = nullptr; }
            
        private:
            
            EditorEntity* _entity = nullptr;
            
        };
        
    }
}









