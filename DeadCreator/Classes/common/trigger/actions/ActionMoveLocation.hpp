//
//  ActionMoveLocation.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 12..
//
//

#pragma once

#include "ActionBase.hpp"
#include "TriggerParameters.hpp"
#include "GameResource.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class ActionMoveLocation : public ActionBase
        {
            
        public:
            
            ActionMoveLocation() { name() = "Move Location"; }
            ActionMoveLocation(const ActionMoveLocation& rhs) : ActionBase(rhs) { copyFrom(rhs); }
            ActionMoveLocation& operator=(const ActionMoveLocation& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ActionMoveLocation& rhs)
            {
                ActionBase::copyFrom(rhs);
                _destLocation = rhs._destLocation;
                _entity = rhs._entity;
                _playerType = rhs._playerType;
                _sourceLocation = rhs._sourceLocation;
            }
            
            virtual ~ActionMoveLocation() = default;
            virtual bool drawEditMode(void* opt) override
            {
                ImGui::BeginChild("##dummy", ImVec2(0, 250), true);
                
                ImGui::Text("Center location labeled");
                
                // dest location combo box
                ImGui::PushID(0);
                ImGui::SameLine(); _destLocation.drawImGui(opt);
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::Text("on");
                
                // entity combo box
                ImGui::PushID(1);
                ImGui::SameLine(); _entity.drawImGui();
                ImGui::PopID();
                
                ImGui::Text("owned by");
                
                // player combo box
                ImGui::PushID(2);
                ImGui::SameLine(); _playerType.drawImGui();
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::Text("at");
                
                // location combo box
                ImGui::PushID(3);
                ImGui::SameLine(); _sourceLocation.drawImGui(opt);
                ImGui::PopID();
                
                ImGui::EndChild();
                
                return (_destLocation.isItemSelected() &&
                        _entity.isItemSelected() &&
                        _playerType.isItemSelected() &&
                        _sourceLocation.isItemSelected());
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret = "Center location labeled ";
                ret += "'" + _destLocation.getParameterName() + "' ";
                ret += "on ";
                ret += "'" + _entity.getParameterName() + "'";
                ret += "owned by\n";
                ret += "'" + _playerType.getParameterName() + "' ";
                ret += "at ";
                ret += "'" + _sourceLocation.getParameterName() + "'";
                return ret;
            }
            
            virtual void reset() override
            {
                _destLocation.reset();
                _entity.reset();
                _playerType.reset();
                _sourceLocation.reset();
            }
            
            virtual ActionMoveLocation* clone() const override { return new ActionMoveLocation(*this); }
            
            virtual flatbuffers::Offset<DeadCreator::Action> getActionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto obj = DeadCreator::CreateMoveLocation(builder,
                                                           builder.CreateString(_destLocation.getLocation()->getLocationName()),
                                                           static_cast<int>(_entity.getEntityType()),
                                                           static_cast<int>(_playerType.getPlayerType()),
                                                           builder.CreateString(_sourceLocation.getLocation()->getLocationName()));
                
                return DeadCreator::CreateAction(builder, DeadCreator::ActionBase_MoveLocation, obj.Union());
            }
            
            void setDestLocation(LocationNode* loc) { _destLocation.setLocation(loc); }
            void setEntity(EntityType type) { _entity.setEntityType(type); }
            void setPlayerType(PlayerType type) { _playerType.setPlayerType(type); }
            void setSourceLocation(LocationNode* loc) { _sourceLocation.setLocation(loc); }
            
        private:
            
            TriggerParameterLocation _destLocation;
            TriggerParameterEntity _entity;
            TriggerParameterPlayerType _playerType;
            TriggerParameterLocation _sourceLocation;
            
        };
        
    }
    
    namespace client
    {
        
        struct ActionMoveLocationData: public TriggerDataBase
        {
            std::string destLocation;
            EntityType entity;
            PlayerType player;
            std::string sourceLocation;
            
            ActionMoveLocationData() { type = TriggerComponentType::ACTION_MOVE_LOCATION; }
        };
        
        class ActionMoveLocation : public ActionBase
        {
            
        public:
            
            explicit ActionMoveLocation(Game* game) : ActionBase(game)
            {}
            
            virtual ~ActionMoveLocation() = default;
            
            static ActionMoveLocation* create(Game* game,
                                              const std::string& destLocation,
                                              EntityType entity, PlayerType player,
                                              const std::string& sourceLocation)
            {
                auto ret = new (std::nothrow) ActionMoveLocation(game);
                if ( ret && ret->init(destLocation, entity, player, sourceLocation) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init(const std::string& destLocation, EntityType entity, PlayerType player, const std::string& sourceLocation)
            {
                _params.destLocation = destLocation;
                _params.entity = entity;
                _params.player = player;
                _params.sourceLocation = sourceLocation;
                
                return true;
            }
            
            virtual void doAction()
            {
                if ( _params.player == PlayerType::CURRENT_PLAYER ) _maskedPlayer = _owner->getPlayers();
                else _maskedPlayer.set(static_cast<int>(_params.player));
                
                const auto& entities = _game->getEntityManager()->getEntities();
                const auto& locations = _game->getGameResource()->getLocations();
                for ( const auto& entity : entities )
                {
                    auto currEntity = entity.second;
                    int player = static_cast<int>(currEntity->getPlayerType());
                    
                    if (_maskedPlayer.test(player) &&
                        currEntity->getEntityType() == _params.entity &&
                        locations.at(_params.sourceLocation).intersectsCircle(currEntity->getWorldPosition(), currEntity->getBoundingRadius()))
                    {
                        cocos2d::Rect updateRect(currEntity->getWorldPosition().x - locations.at(_params.sourceLocation).size.width / 2,
                                                 currEntity->getWorldPosition().y - locations.at(_params.sourceLocation).size.height / 2,
                                                 locations.at(_params.sourceLocation).size.width,
                                                 locations.at(_params.sourceLocation).size.height);
                        
                        _game->getGameResource()->updateLocation(_params.destLocation, updateRect);
                        
                        _game->addLog("move location");
                        
                        break;
                    }
                }
            }
            
        private:
            
            ActionMoveLocationData _params;
            std::bitset<30> _maskedPlayer;
            
        };
        
    }
}









