//
//  ActionPauseGame.hpp
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
        
        class ActionPauseGame : public ActionBase
        {
            
        public:
            
            explicit ActionPauseGame(GMXLayer* gmxLayer) : ActionBase(gmxLayer) { name() = "Pause Game"; }
            ActionPauseGame(const ActionPauseGame& rhs) : ActionBase(rhs) { copyFrom(rhs); }
            ActionPauseGame& operator=(const ActionPauseGame& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ActionPauseGame& rhs)
            {
                ActionBase::copyFrom(rhs);
            }
            
            virtual ~ActionPauseGame() = default;
            virtual bool drawEditMode(void* opt) override
            {
                ImGui::TextUnformatted("Pause the game.");
                
                return true;
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret = "Pause the game.";
                return ret;
            }
            
            virtual void reset() override { }
            virtual ActionPauseGame* clone() const override { return new ActionPauseGame(*this); }
            
            virtual flatbuffers::Offset<DeadCreator::Action> getActionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto obj = DeadCreator::CreatePauseGame(builder);
                return DeadCreator::CreateAction(builder, DeadCreator::ActionBase_PauseGame, obj.Union());
            }
            
        };
        
    }
    
    namespace client
    {
        
        struct ActionPauseGameData: public TriggerDataBase
        {
            ActionPauseGameData() { type = TriggerComponentType::ACTION_PAUSE_GAME; }
        };
        
        class ActionPauseGame : public ActionBase
        {
            
        public:
            
            explicit ActionPauseGame(Game* game) : ActionBase(game)
            {}
            
            virtual ~ActionPauseGame() = default;
            
            static ActionPauseGame* create(Game* game)
            {
                auto ret = new (std::nothrow) ActionPauseGame(game);
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
                _game->pushLogic(0.0, MessageType::PAUSE_GAME, nullptr);
            }
            
        private:
            
            ActionPauseGameData _params;
            
        };
        
    }
}









