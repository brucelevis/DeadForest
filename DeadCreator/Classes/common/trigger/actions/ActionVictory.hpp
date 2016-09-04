//
//  ActionVictory.hpp
//  DeadCreator
//
//  Created by mac on 2016. 9. 5..
//
//

#pragma once

#include "ActionBase.hpp"
#include "TriggerParameters.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class ActionVictory : public ActionBase
        {
            
        public:
            
            ActionVictory() { name() = "Victory"; }
            ActionVictory(const ActionVictory& rhs) : ActionBase(rhs) { copyFrom(rhs); }
            ActionVictory& operator=(const ActionVictory& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ActionVictory& rhs)
            {
                ActionBase::copyFrom(rhs);
            }
            
            virtual ~ActionVictory() = default;
            virtual bool drawEditMode(void* opt) override
            {
                ImGui::BeginChild("##dummy", ImVec2(0, 250), true);
                ImGui::TextUnformatted("End scenario in victory for current player.");
                ImGui::EndChild();
                
                return true;
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret = "End scenario in victory for current player.";
                return ret;
            }
            
            virtual void reset() override { }
            virtual ActionVictory* clone() const override { return new ActionVictory(*this); }
            
            virtual flatbuffers::Offset<DeadCreator::Action> getActionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto obj = DeadCreator::CreateVictory(builder);
                return DeadCreator::CreateAction(builder, DeadCreator::ActionBase_Victory, obj.Union());
            }
            
        };
        
    }
    
    namespace client
    {
        
        struct ActionVictoryData: public TriggerDataBase
        {
            ActionVictoryData() { type = TriggerComponentType::ACTION_VICTORY; }
        };
        
        class ActionVictory : public ActionBase
        {
            
        public:
            
            explicit ActionVictory(Game* game) : ActionBase(game)
            {}
            
            virtual ~ActionVictory() = default;
            
            static ActionVictory* create(Game* game)
            {
                auto ret = new (std::nothrow) ActionVictory(game);
                if ( ret && ret->init() )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init() { return true; }
            
            virtual void doAction() { cocos2d::log("victory"); }
            
        private:
            
            ActionVictoryData _params;
            
        };
        
    }
}









