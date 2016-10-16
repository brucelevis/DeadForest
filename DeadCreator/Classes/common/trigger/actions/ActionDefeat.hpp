//
//  ActionDefeat.hpp
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
        
        class ActionDefeat : public ActionBase
        {
            
        public:
            
            ActionDefeat() { name() = "Defeat"; }
            ActionDefeat(const ActionDefeat& rhs) : ActionBase(rhs) { copyFrom(rhs); }
            ActionDefeat& operator=(const ActionDefeat& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ActionDefeat& rhs)
            {
                ActionBase::copyFrom(rhs);
            }
            
            virtual ~ActionDefeat() = default;
            virtual bool drawEditMode(void* opt) override
            {
                ImGui::TextUnformatted("End scenario in defeat for current player.");
                
                return true;
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret = "End scenario in defeat for current player.";
                return ret;
            }
            
            virtual void reset() override { }
            virtual ActionDefeat* clone() const override { return new ActionDefeat(*this); }
            
            virtual flatbuffers::Offset<DeadCreator::Action> getActionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto obj = DeadCreator::CreateDefeat(builder);
                return DeadCreator::CreateAction(builder, DeadCreator::ActionBase_Defeat, obj.Union());
            }
            
        };
        
    }
    
    namespace client
    {
        
        struct ActionDefeatData: public TriggerDataBase
        {
            ActionDefeatData() { type = TriggerComponentType::ACTION_DEFEAT; }
        };
        
        class ActionDefeat : public ActionBase
        {
            
        public:
            
            explicit ActionDefeat(Game* game) : ActionBase(game)
            {}
            
            virtual ~ActionDefeat() = default;
            
            static ActionDefeat* create(Game* game)
            {
                auto ret = new (std::nothrow) ActionDefeat(game);
                if ( ret && ret->init() )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init() { return true; }
            
            virtual void doAction()
            {
                auto players = _owner->getPlayers();
                if ( players.test(static_cast<int>(_game->getPlayerPtr()->getPlayerType())) )
                {
                    _game->replaceDefeatScene(3.0f);
                }
            }
            
        private:
            
            ActionDefeatData _params;
            
        };
        
    }
}









