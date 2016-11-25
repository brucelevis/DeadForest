//
//  RemoveEntityToolCommand.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 5..
//
//

#pragma once

#include <vector>

#include "CommandBase.hpp"
#include "EditorEntity.hpp"
#include "GMXLayer.hpp"

namespace realtrick
{
    namespace editor
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
            
            virtual ~RemoveEntityToolCommand() { _entities.clear(); }
            virtual void execute() override
            {
                for( auto& ent : _entities )
                {
                    _layer->eraseEntity(ent->getID(), true);
                }
            }
            
            virtual void undo() override
            {
                for( auto& ent : _entities )
                {
                    _layer->addEntity(ent, ent->getLocalZOrder(), true);
                }
            }
            
            virtual RemoveEntityToolCommand* clone() const override { return new RemoveEntityToolCommand(*this); }
            virtual bool empty() const override { return _entities.empty(); }
            void pushEntity(const std::vector<EditorEntity*>& entities) { if ( _isBegan ) _entities = entities; }
            
        private:
            
            virtual void beginImpl() override { _entities.clear(); }
            virtual void endImpl() override { _entities.clear(); }
            
        private:
            
            std::vector<EditorEntity*> _entities;
            
        };
        
    }
}









