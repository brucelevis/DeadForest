//
//  ActionSetAlianceStatus.hpp
//  DeadCreator
//
//  Created by mac on 2016. 11. 14..
//
//


#pragma once

#include "ActionBase.hpp"
#include "TriggerParameters.hpp"
#include "Game.hpp"


namespace realtrick
{
    namespace editor
    {
        
        class GMXLayer;
        
        class ActionSetAllianceStatus : public ActionBase
        {
            
        public:
            
            explicit ActionSetAllianceStatus(GMXLayer* gmxLayer) : ActionBase(gmxLayer),
            _playerType(gmxLayer),
            _allyStatus(gmxLayer)
            {
                name() = "Set Alliance Status";
            }
            ActionSetAllianceStatus(const ActionSetAllianceStatus& rhs) : ActionBase(rhs) { copyFrom(rhs); }
            ActionSetAllianceStatus& operator=(const ActionSetAllianceStatus& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ActionSetAllianceStatus& rhs)
            {
                ActionBase::copyFrom(rhs);
                _playerType = rhs._playerType;
                _allyStatus = rhs._allyStatus;
            }
            
            virtual ~ActionSetAllianceStatus() = default;
            virtual bool drawEditMode(void* opt) override
            {
                // draw imgui
                ImGui::TextUnformatted("Set");
                
                ImGui::PushID(0);
                ImGui::SameLine(); _playerType.drawImGui();
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::TextUnformatted("to");
                
                ImGui::PushID(1);
                ImGui::SameLine(); _allyStatus.drawImGui();
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::TextUnformatted(".");
                
                return (_playerType.isItemSelected() && _allyStatus.isItemSelected());
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret;
                ret += "Set ";
                ret += "'" + _playerType.getParameterName() + "' ";
                ret += "to ";
                ret += "'" + _allyStatus.getParameterName() + "'.";
                
                return ret;
            }
            
            virtual void reset() override { }
            virtual ActionSetAllianceStatus* clone() const override { return new ActionSetAllianceStatus(*this); }
            
            virtual flatbuffers::Offset<DeadCreator::Action> getActionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto obj = DeadCreator::CreateSetAllianceStatus(builder,
                                                                static_cast<int32_t>(_playerType.getPlayerType()),
                                                                _allyStatus.getAllyStatusType() == AllyStatusType::ALLY ? true : false);
                
                return DeadCreator::CreateAction(builder, DeadCreator::ActionBase_SetAllianceStatus, obj.Union());
            }
            
            void setAllianceStatus(AllyStatusType type) { _allyStatus.setAllyStatusType(type); }
            void setPlayerType(PlayerType player) { _playerType.setPlayerType(player); }
            
        private:
            
            TriggerParameterPlayerType _playerType;
            TriggerParameterAllyStatus _allyStatus;
            
        };
        
    }
    
    namespace client
    {
        
        struct ActionSetAllianceStatusData: public TriggerDataBase
        {
            PlayerType player;
            bool isAlly;
            ActionSetAllianceStatusData() { type = TriggerComponentType::ACTION_SET_ALLIANCE_STATUS; }
        };
        
        class ActionSetAllianceStatus : public ActionBase
        {
            
        public:
            
            explicit ActionSetAllianceStatus(Game* game) : ActionBase(game)
            {}
            
            virtual ~ActionSetAllianceStatus() = default;
            
            static ActionSetAllianceStatus* create(Game* game, PlayerType player, bool isAlly)
            {
                auto ret = new (std::nothrow) ActionSetAllianceStatus(game);
                if ( ret && ret->init(player, isAlly) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init(PlayerType playerType, bool isAlly)
            {
                _params.player = playerType;
                _params.isAlly = isAlly;
                
                return true;
            }
            
            virtual void doAction()
            {
                auto players = _owner->getPlayers();
                for(int i = 1 ; i <= 8 ; ++ i)
                {
                    if ( !players.test(i) ) continue;
                    
                    _game->setAllyState(static_cast<PlayerType>(i), _params.player, true);
                }
            }
            
        private:
            
            ActionSetAllianceStatusData _params;
            
        };
        
    }
}
