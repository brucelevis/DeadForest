//
//  ActionResumeGame.hpp
//  DeadCreator
//
//  Created by mac on 2016. 9. 5..
//
//

#pragma once

#include "ActionBase.hpp"
#include "TriggerParameters.hpp"
#include "MessageTypes.hpp"


namespace realtrick
{
    namespace editor
    {
        
        class GMXLayer;
        
        class ActionResumeGame : public ActionBase
        {
            
        public:
            
            explicit ActionResumeGame(GMXLayer* gmxLayer) : ActionBase(gmxLayer) { name() = "Resume Game"; }
            ActionResumeGame(const ActionResumeGame& rhs) : ActionBase(rhs) { copyFrom(rhs); }
            ActionResumeGame& operator=(const ActionResumeGame& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ActionResumeGame& rhs)
            {
                ActionBase::copyFrom(rhs);
            }
            
            virtual ~ActionResumeGame() = default;
            virtual bool drawEditMode(void* opt) override
            {
                ImGui::TextUnformatted("Resume the game.");
                
                return true;
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret = "Resume the game.";
                return ret;
            }
            
            virtual void reset() override { }
            virtual ActionResumeGame* clone() const override { return new ActionResumeGame(*this); }
            
            virtual flatbuffers::Offset<DeadCreator::Action> getActionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto obj = DeadCreator::CreateResumeGame(builder);
                return DeadCreator::CreateAction(builder, DeadCreator::ActionBase_ResumeGame, obj.Union());
            }
            
        };
        
    }
    
    namespace client
    {
        
        struct ActionResumeGameData: public TriggerDataBase
        {
            ActionResumeGameData() { type = TriggerComponentType::ACTION_RESUME_GAME; }
        };
        
        class ActionResumeGame : public ActionBase
        {
            
        public:
            
            explicit ActionResumeGame(Game* game) : ActionBase(game)
            {}
            
            virtual ~ActionResumeGame() = default;
            
            static ActionResumeGame* create(Game* game)
            {
                auto ret = new (std::nothrow) ActionResumeGame(game);
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
                _game->pushLogic(0.0, MessageType::RESUME_GAME, nullptr);
            }
            
        private:
            
            ActionResumeGameData _params;
            
        };
        
    }
}









