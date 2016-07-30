//
//  ActionBase.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 18..
//
//

#pragma once

#include <vector>

#include "cocos2d.h"

#include "TriggerComponentProtocol.hpp"
#include "TriggerParameterBase.hpp"

#include "imgui.h"
#include "imgui_internal.h"

#include "GMXFile_generated.h"

namespace realtrick
{
    namespace editor
    {
        
        class ActionBase : public TriggerComponentProtocol
        {
            
        public:
            
            ActionBase() = default;
            ActionBase(const ActionBase& rhs) : TriggerComponentProtocol(rhs) { copyFrom(rhs); }
            ActionBase& operator=(const ActionBase& rhs)
            {
                if ( &rhs != this) copyFrom(rhs);
                return *this;
            }
            virtual ~ActionBase() = default;
            void copyFrom(const ActionBase& rhs) { TriggerComponentProtocol::copyFrom(rhs); }
            
            virtual bool drawEditMode(void* opt) override { return false; }
            virtual std::string getSummaryString() const override { return ""; }
            virtual void reset() override {}
            virtual ActionBase* clone() const override { return nullptr; }
            virtual void deepCopy(TriggerComponentProtocol* copy) override { TriggerComponentProtocol::deepCopy(copy); }
            
            virtual flatbuffers::Offset<DeadCreator::Action> getActionObject(flatbuffers::FlatBufferBuilder& builder) = 0;
            
        };
        
    }
    
    namespace client
    {
        
        class GameTrigger;
        
        class ActionBase : public cocos2d::Ref
        {
           
        public:
            
            explicit ActionBase(GameTrigger* owner) : _owner(owner)
            {}
            
            virtual ~ActionBase() = default;
            
            virtual void doAction() = 0;
            
        protected:
            
            GameTrigger* _owner;
            
        };
        
    };
    
};









