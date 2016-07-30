//
//  ConditionBase.hpp
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
        
        class ConditionBase : public TriggerComponentProtocol
        {
            
        public:
            
            ConditionBase() = default;
            ConditionBase(const ConditionBase& rhs) : TriggerComponentProtocol(rhs) { copyFrom(rhs); }
            ConditionBase& operator=(const ConditionBase& rhs)
            {
                if ( &rhs != this) copyFrom(rhs);
                return *this;
            }
            virtual ~ConditionBase() = default;
            void copyFrom(const ConditionBase& rhs) { TriggerComponentProtocol::copyFrom(rhs); }
            
            virtual bool drawEditMode(void* opt) override { return false; }
            virtual std::string getSummaryString() const override { return ""; }
            virtual void reset() override {}
            virtual ConditionBase* clone() const override { return nullptr; }
            virtual void deepCopy(TriggerComponentProtocol* copy) override { TriggerComponentProtocol::deepCopy(copy); }
            
            virtual flatbuffers::Offset<DeadCreator::Condition> getConditionObject(flatbuffers::FlatBufferBuilder& builder) = 0;
            
        };
        
    }
    
    namespace client
    {
        
        class GameManager;
        class GameTrigger;
        
        class ConditionBase : public cocos2d::Ref
        {
            
        public:
            
            explicit ConditionBase(GameManager* mgr) : _gameMgr(mgr)
            {}
            
            virtual ~ConditionBase() { _owner = nullptr; _gameMgr = nullptr; }
            
            virtual bool isReady() = 0;
            void setOwner(GameTrigger* owner) { _owner = owner; }
            
        protected:
            
            GameTrigger* _owner;
            GameManager* _gameMgr;
            
        };
        
    }
    
};









