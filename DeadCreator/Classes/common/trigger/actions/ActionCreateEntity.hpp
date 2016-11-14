//
//  ActionCreateEntity.hpp
//  DeadCreator
//
//  Created by mac on 2016. 11. 13..
//
//

#pragma once

#include "ActionBase.hpp"
#include "TriggerParameters.hpp"
#include "GameResource.hpp"
#include "EntityManager.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class GMXLayer;
        
        class ActionCreateEntity : public ActionBase
        {
            
        public:
            
            explicit ActionCreateEntity(GMXLayer* gmxLayer) : ActionBase(gmxLayer),
            _number(gmxLayer),
            _entity(gmxLayer),
            _playerType(gmxLayer),
            _location(gmxLayer)
            {
                name() = "Create Entity";
            }
            ActionCreateEntity(const ActionCreateEntity& rhs) : ActionBase(rhs) { copyFrom(rhs); }
            ActionCreateEntity& operator=(const ActionCreateEntity& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ActionCreateEntity& rhs)
            {
                ActionBase::copyFrom(rhs);
                _number = rhs._number;
                _entity = rhs._entity;
                _playerType = rhs._playerType;
                _location = rhs._location;
            }
            
            virtual ~ActionCreateEntity() = default;
            virtual bool drawEditMode(void* opt) override
            {
                
                ImGui::Text("Create");
                
                // number(all) item
                ImGui::PushID(0);
                ImGui::SameLine(); _number.drawImGui();
                ImGui::PopID();
                
                // entity combo box
                ImGui::PushID(1);
                ImGui::SameLine(); _entity.drawImGui();
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::Text("at");
                
                // location combo box
                ImGui::PushID(2);
                ImGui::SameLine(); _location.drawImGui(opt);
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::Text("for");
                
                // player combo box
                ImGui::PushID(3);
                ImGui::SameLine(); _playerType.drawImGui();
                ImGui::PopID();
                
                return (_number.isItemSelected() &&
                        _entity.isItemSelected() &&
                        _playerType.isItemSelected() &&
                        _location.isItemSelected());
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret = "Create ";
                ret += "'" + _number.getParameterName() + "' ";
                ret += "'" + _entity.getParameterName() + "' ";
                ret += "at ";
                ret += "'" + _location.getParameterName() + "' ";
                ret += "for ";
                ret += "'" + _playerType.getParameterName() + "'";
                return ret;
            }
            
            virtual void reset() override
            {
                _number.reset();
                _entity.reset();
                _playerType.reset();
                _location.reset();
            }
            
            virtual ActionCreateEntity* clone() const override { return new ActionCreateEntity(*this); }
            
            virtual flatbuffers::Offset<DeadCreator::Action> getActionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto obj = DeadCreator::CreateCreateEntity(builder,
                                                           _number.getNumber(),
                                                           static_cast<int>(_entity.getEntityType()),
                                                           builder.CreateString(_location.getLocation()->getLocationName()),
                                                           static_cast<int>(_playerType.getPlayerType()));
                
                return DeadCreator::CreateAction(builder, DeadCreator::ActionBase_CreateEntity, obj.Union());
            }
            
            void setNumber(int number) { _number.setNumber(number); }
            void setEntity(EntityType type) { _entity.setEntityType(type); }
            void setPlayerType(PlayerType type) { _playerType.setPlayerType(type); }
            void setLocation(LocationNode* loc) { _location.setLocation(loc); }
            
        private:
            
            TriggerParameterNumber _number;
            TriggerParameterEntity _entity;
            TriggerParameterPlayerType _playerType;
            TriggerParameterLocation _location;
            
        };
        
    }
    
    namespace client
    {
     
        struct ActionCreateEntityData: public TriggerDataBase
        {
            int number;
            EntityType entity;
            PlayerType player;
            std::string location;
            
            ActionCreateEntityData() { type = TriggerComponentType::ACTION_CREATE_ENTITY; }
        };
        
        class ActionCreateEntity : public ActionBase
        {
            
        public:
            
            explicit ActionCreateEntity(Game* game) : ActionBase(game)
            {}
            
            virtual ~ActionCreateEntity() = default;
            
            static ActionCreateEntity* create(Game* game, int number, EntityType entity, PlayerType player, const std::string& location)
            {
                auto ret = new (std::nothrow) ActionCreateEntity(game);
                if ( ret && ret->init(number, entity, player, location) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init(int number, EntityType entity, PlayerType player, const std::string& location)
            {
                _params.number = number;
                _params.entity = entity;
                _params.player = player;
                _params.location = location;
                
                return true;
            }
            
            virtual void doAction()
            {
                if ( _params.player == PlayerType::CURRENT_PLAYER ) _maskedPlayer = _owner->getPlayers();
                else _maskedPlayer.set(static_cast<int>(_params.player));
                
                auto location = _game->getGameResource()->getLocations().at(_params.location);
                auto center = cocos2d::Vec2(location.getMidX() + cocos2d::random(-5.0f, 5.0f),
                                            location.getMidY() + cocos2d::random(-5.0f, 5.0f));
                
                for(int i = 1 ; i <= 8 ; ++ i)
                {
                    if ( !_maskedPlayer.test(i) ) continue;
                    
                    EntityData data;
                    data.id = _game->getEntityManager()->getNextValidID();
                    data.entityType = _params.entity;
                    data.playerType = static_cast<PlayerType>(i);
                    data.position = center;
                    _game->getEntityManager()->addEntityWithData(&data);
                }
            }
            
        private:
            
            ActionCreateEntityData _params;
            std::bitset<30> _maskedPlayer;
            
        };

        
    }
}









