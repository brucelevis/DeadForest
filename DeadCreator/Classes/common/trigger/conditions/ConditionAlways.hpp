//
//  ConditionAlways.hpp
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
        
        class ConditionAlways : public ConditionBase
        {
            
        public:
            
            ConditionAlways() { name() = "Always"; }
            ConditionAlways(const ConditionAlways& rhs) : ConditionBase(rhs) { copyFrom(rhs); }
            ConditionAlways& operator=(const ConditionAlways& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ConditionAlways& rhs)
            {
                ConditionBase::copyFrom(rhs);
            }
            
            virtual ~ConditionAlways() = default;
            virtual bool drawEditMode(void* opt) override
            {
                ImGui::TextUnformatted("Always.");
                
                return true;
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret = "Always";
                return ret;
            }
            
            virtual void reset() override {}
            
            virtual ConditionAlways* clone() const override
            {
                return new ConditionAlways(*this);
            }
            
            virtual flatbuffers::Offset<DeadCreator::Condition> getConditionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto obj = DeadCreator::CreateAlways(builder);
                return DeadCreator::CreateCondition(builder, DeadCreator::ConditionBase_Always, obj.Union());
            }
            
        };
        
    }
    
    namespace client
    {
        
        struct ConditionAlwaysData: public TriggerDataBase
        {
            ConditionAlwaysData() { type = TriggerComponentType::CONDITION_ALWAYS; }
        };
        
        class ConditionAlways : public ConditionBase
        {
            
        public:
            
            explicit ConditionAlways(Game* game) : ConditionBase(game)
            {}
            virtual ~ConditionAlways() = default;
            
            static ConditionAlways* create(Game* game)
            {
                auto ret = new (std::nothrow) ConditionAlways(game);
                if ( ret && ret->init() )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init() { return true; }
            virtual bool isReady() override
            {
                if ( _game->isPaused() ) return false;
                
                return true;
            }
            
        private:
            
            ConditionAlwaysData _params;
            
        };
        
    }
    
}









