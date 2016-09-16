//
//  ActionPlaySoundAtLocation.hpp
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
        
        class ActionPlaySoundAtLocation : public ActionBase
        {
            
        public:
            
            ActionPlaySoundAtLocation() { name() = "Play Sound At Location"; _fileName.reset(); }
            ActionPlaySoundAtLocation(const ActionPlaySoundAtLocation& rhs) : ActionBase(rhs) { copyFrom(rhs); }
            ActionPlaySoundAtLocation& operator=(const ActionPlaySoundAtLocation& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ActionPlaySoundAtLocation& rhs)
            {
                ActionBase::copyFrom(rhs);
                _fileName = rhs._fileName;
                _location = rhs._location;
            }
            
            virtual ~ActionPlaySoundAtLocation() = default;
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
                
                ImGui::SameLine(); ImGui::Text("(file_name.mp3) at");
                
                // location combo box
                ImGui::PushID(1);
                ImGui::SameLine(); _location.drawImGui(opt);
                ImGui::PopID();
                
                ImGui::EndChild();
                
                return (_fileName.isItemSelected() &&
                        _location.isItemSelected());
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret = "Play ";
                ret += "'" + _fileName.getParameterName() + "' ";
                ret += "at ";
                ret += "'" + _location.getParameterName() + "'";
                return ret;
            }
            
            virtual void reset() override
            {
                _fileName.reset();
                _location.reset();
            }
            
            virtual ActionPlaySoundAtLocation* clone() const override { return new ActionPlaySoundAtLocation(*this); }
            
            virtual flatbuffers::Offset<DeadCreator::Action> getActionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto obj = DeadCreator::CreatePlaySoundAtLocation(builder,
                                                                  builder.CreateString(_fileName.getText()),
                                                                  builder.CreateString(_location.getLocation()->getLocationName()));
                
                return DeadCreator::CreateAction(builder, DeadCreator::ActionBase_PlaySoundAtLocation, obj.Union());
            }
            
            void setFileName(const std::string& fileName) { _fileName.setText(fileName); }
            void setLocation(LocationNode* loc) { _location.setLocation(loc); }
            
        private:
            
            TriggerParameterText _fileName;
            TriggerParameterLocation _location;
            
        };
        
    }
    
    
    namespace client
    {
        
        struct ActionPlaySoundAtLocationData: public TriggerDataBase
        {
            std::string fileName;
            std::string location;
            
            ActionPlaySoundAtLocationData() { type = TriggerComponentType::ACTION_PLAY_SOUND_AT_LOCATION; }
        };
        
        class ActionPlaySoundAtLocation : public ActionBase
        {
            
        public:
            
            explicit ActionPlaySoundAtLocation(Game* game) : ActionBase(game)
            {}
            
            virtual ~ActionPlaySoundAtLocation() = default;
            
            static ActionPlaySoundAtLocation* create(Game* game, const std::string& fileName, const std::string& location)
            {
                auto ret = new (std::nothrow) ActionPlaySoundAtLocation(game);
                if ( ret && ret->init(fileName, location) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init(const std::string& fileName, const std::string& location)
            {
                _params.fileName = fileName;
                _params.location = location;
                
                return true;
            }
            
            virtual void doAction()
            {
                _maskedPlayer = _owner->getPlayers();
                const auto& locations = _game->getGameResource()->getLocations();
                
                if ( _maskedPlayer.test(static_cast<int>(_game->getPlayerPtr()->getPlayerType())) )
                {
                    SoundSource s;
                    s.fileName = _params.fileName;
                    s.position = cocos2d::Vec2(locations.at(_params.location).getMidX(), locations.at(_params.location).getMidY());
                    s.soundRange = 1000.0f;
                    s.volume = 1.0f;
                    _game->sendMessage(0.0, _game->getPlayerPtr(), nullptr, MessageType::PLAY_SOUND, &s);
                }
            }
            
        private:
            
            ActionPlaySoundAtLocationData _params;
            std::bitset<30> _maskedPlayer;
            
        };
        
    }
    
}









