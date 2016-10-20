//
//  ActionPreserveTrigger.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 4..
//
//

#pragma once

#include "ActionBase.hpp"
#include "TriggerParameters.hpp"


namespace realtrick
{
    namespace editor
    {
     
        class GMXLayer;
        
        class ActionPreserveTrigger : public ActionBase
        {
            
        public:
            
            explicit ActionPreserveTrigger(GMXLayer* gmxLayer) : ActionBase(gmxLayer) { name() = "Preserve Trigger"; }
            ActionPreserveTrigger(const ActionPreserveTrigger& rhs) : ActionBase(rhs) { copyFrom(rhs); }
            ActionPreserveTrigger& operator=(const ActionPreserveTrigger& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ActionPreserveTrigger& rhs)
            {
                ActionBase::copyFrom(rhs);
            }
            
            virtual ~ActionPreserveTrigger() = default;
            virtual bool drawEditMode(void* opt) override
            {
                ImGui::TextUnformatted("Preserve trigger.");
                
                return true;
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret = "Preserve trigger.";
                return ret;
            }
            
            virtual void reset() override { }
            virtual ActionPreserveTrigger* clone() const override { return new ActionPreserveTrigger(*this); }
            
            virtual flatbuffers::Offset<DeadCreator::Action> getActionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto obj = DeadCreator::CreatePreserveTrigger(builder);
                return DeadCreator::CreateAction(builder, DeadCreator::ActionBase_PreserveTrigger, obj.Union());
            }
            
        };
        
    }
    
    namespace client
    {
        
        struct ActionPreserveTriggerData: public TriggerDataBase
        {
            ActionPreserveTriggerData() { type = TriggerComponentType::ACTION_PRESERVE_TRIGGER; }
        };
        
        class ActionPreserveTrigger : public ActionBase
        {
            
        public:
            
            explicit ActionPreserveTrigger(Game* game) : ActionBase(game)
            {}
            
            virtual ~ActionPreserveTrigger() = default;
            
            static ActionPreserveTrigger* create(Game* game)
            {
                auto ret = new (std::nothrow) ActionPreserveTrigger(game);
                if ( ret && ret->init() )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init() { return true; }
            
            virtual void doAction() { _owner->setPreserveTrigger(true); }
            
        private:
            
            ActionPreserveTriggerData _params;
            
        };
        
    }
}









