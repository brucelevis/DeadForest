//
//  ConditionNever.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 4..
//
//

#pragma once

#include "TriggerParameters.hpp"
#include "EditorEntity.hpp"
#include "GMXLayer.hpp"
#include "GameTrigger.hpp"
#include "EntityManager.hpp"

#include "Game.hpp"
#include "EntityBase.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class ConditionNever : public ConditionBase
        {
            
        public:
            
            ConditionNever() { name() = "Never"; }
            ConditionNever(const ConditionNever& rhs) : ConditionBase(rhs) { copyFrom(rhs); }
            ConditionNever& operator=(const ConditionNever& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ConditionNever& rhs)
            {
                ConditionBase::copyFrom(rhs);
            }
            
            virtual ~ConditionNever() = default;
            virtual bool drawEditMode(void* opt) override
            {
                ImGui::BeginChild("##dummy", ImVec2(0, 250), true);
                ImGui::TextUnformatted("Never.");
                ImGui::EndChild();
                
                return true;
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret = "Never";
                return ret;
            }
            
            virtual void reset() override {}
            
            virtual ConditionNever* clone() const override
            {
                return new ConditionNever(*this);
            }
            
            virtual void deepCopy(TriggerComponentProtocol* copy) override
            {
                ConditionBase::deepCopy(copy);
            }
            
            virtual flatbuffers::Offset<DeadCreator::Condition> getConditionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto obj = DeadCreator::CreateNever(builder);
                return DeadCreator::CreateCondition(builder, DeadCreator::ConditionBase_Never, obj.Union());
            }
            
        };
        
    }
    
    namespace client
    {
        
        struct ConditionNeverData: public TriggerDataBase
        {
            ConditionNeverData() { type = TriggerComponentType::CONDITION_NEVER; }
        };
        
        class ConditionNever : public ConditionBase
        {
            
        public:
            
            explicit ConditionNever(Game* game) : ConditionBase(game)
            {}
            virtual ~ConditionNever() = default;
            
            static ConditionNever* create(Game* game)
            {
                auto ret = new (std::nothrow) ConditionNever(game);
                if ( ret && ret->init() )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init() { return true; }
            virtual bool isReady() override { return false; }
            
        private:
            
            ConditionNeverData _params;
            
        };
        
    }
}









