//
//  ActionPlaySound.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 12..
//
//

#pragma once

#include "ActionBase.hpp"
#include "TriggerParameters.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class ActionPlaySound : public ActionBase
        {
            
        public:
            
            ActionPlaySound() { name() = "Play Sound"; _fileName.reset(); }
            ActionPlaySound(const ActionPlaySound& rhs) : ActionBase(rhs) { copyFrom(rhs); }
            ActionPlaySound& operator=(const ActionPlaySound& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ActionPlaySound& rhs)
            {
                ActionBase::copyFrom(rhs);
                _fileName = rhs._fileName;
            }
            
            virtual ~ActionPlaySound() = default;
            virtual bool drawEditMode(void* opt) override
            {
                ImGui::BeginChild("##dummy", ImVec2(0, 250), true);
                
                ImGui::Text("Play");
                
                // file name
                ImGui::PushID(0);
                ImGui::PushItemWidth(200);
                ImGui::SameLine(); _fileName.drawImGui();
                ImGui::PopItemWidth();
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::Text("(file_name.mp3)");
                
                ImGui::EndChild();
                
                return (_fileName.isItemSelected());
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret = "Play ";
                ret += "'" + _fileName.getParameterName() + "' ";
                return ret;
            }
            
            virtual void reset() override
            {
                _fileName.reset();
            }
            
            virtual ActionPlaySound* clone() const override { return new ActionPlaySound(*this); }
            
            virtual flatbuffers::Offset<DeadCreator::Action> getActionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto obj = DeadCreator::CreatePlaySound(builder, builder.CreateString(_fileName.getText()));
                return DeadCreator::CreateAction(builder, DeadCreator::ActionBase_PlaySound, obj.Union());
            }
            
            void setFileName(const std::string& fileName) { _fileName.setText(fileName); }
            
        private:
            
            TriggerParameterText _fileName;
            
        };
        
    }
    
    
    namespace client
    {
        
        struct ActionPlaySoundData: public TriggerDataBase
        {
            std::string fileName;
            
            ActionPlaySoundData() { type = TriggerComponentType::ACTION_PLAY_SOUND; }
        };
        
        class ActionPlaySound : public ActionBase
        {
            
        public:
            
            explicit ActionPlaySound(Game* game) : ActionBase(game)
            {}
            
            virtual ~ActionPlaySound() = default;
            
            static ActionPlaySound* create(Game* game, const std::string& fileName)
            {
                auto ret = new (std::nothrow) ActionPlaySound(game);
                if ( ret && ret->init(fileName) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init(const std::string& fileName)
            {
                _params.fileName = fileName;
                
                return true;
            }
            
            virtual void doAction()
            {
                auto players = _owner->getPlayers();
                if ( players.test(static_cast<int>(_game->getPlayerPtr()->getPlayerType())) )
                {
                    SoundSource s;
                    s.fileName = _params.fileName;
                    s.position = cocos2d::Vec2::ZERO;
                    s.soundRange = std::numeric_limits<float>::max();
                    s.volume = 1.0f;
                    _game->sendMessage(0.0, _game->getPlayerPtr(), nullptr, MessageType::PLAY_SOUND, &s);
                    
                    _game->addLog(cocos2d::StringUtils::format("play sound (%s).", _params.fileName.c_str()));
                }
            }
            
        private:
            
            ActionPlaySoundData _params;
            
        };
        
    }
    
}









