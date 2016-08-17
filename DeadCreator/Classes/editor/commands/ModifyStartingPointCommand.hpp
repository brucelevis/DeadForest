//
//  ModifyStartingPointCommand.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
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
        
        class ModiftStartingPointCommand : public CommandBase
        {
            
        public:
            
            explicit ModiftStartingPointCommand(GMXLayer* layer) :
            CommandBase(layer)
            {
                _commandName = "Modify Starting Point";
            }
            
            ModiftStartingPointCommand(const ModiftStartingPointCommand& rhs) : CommandBase(rhs)
            {
                copyFrom(rhs);
            }
            
            void copyFrom(const ModiftStartingPointCommand& rhs)
            {
                _entity = rhs._entity;
                _newPosition = rhs._newPosition;
                _prevPosition = rhs._prevPosition;
            }
            
            virtual ~ModiftStartingPointCommand() = default;
            
            virtual void execute() override { _entity->setPosition(_newPosition); }
            virtual void undo() override { _entity->setPosition(_prevPosition); }
            
            virtual ModiftStartingPointCommand* clone() const override { return new ModiftStartingPointCommand(*this); }
            virtual bool empty() const override { return !_entity; }
            void modifyStartingPoint(EditorEntity* ent, const cocos2d::Vec2& newPosition, const cocos2d::Vec2& prevPosition)
            {
                if ( _isBegan )
                {
                    _entity = ent;
                    _newPosition = newPosition;
                    _prevPosition = prevPosition;
                }
            }
            
        private:
            
            virtual void beginImpl() override { _entity = nullptr; }
            virtual void endImpl() override { _entity = nullptr; }
            
        private:
            
            EditorEntity* _entity = nullptr;
            cocos2d::Vec2 _newPosition;
            cocos2d::Vec2 _prevPosition;
            
        };
        
    }
}









