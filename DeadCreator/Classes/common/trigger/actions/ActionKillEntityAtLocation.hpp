//
//  ActionKillEntityAtLocation.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 8. 4..
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
        
        class GMXLayer;
        
        class ActionKillEntityAtLocation : public ActionBase
        {
            
        public:
            
            explicit ActionKillEntityAtLocation(GMXLayer* gmxLayer) : ActionBase(gmxLayer),
            _numberAll(gmxLayer),
            _entity(gmxLayer),
            _playerType(gmxLayer),
            _location(gmxLayer)
            {
                name() = "Kill Entity At Location";
            }
            ActionKillEntityAtLocation(const ActionKillEntityAtLocation& rhs) : ActionBase(rhs) { copyFrom(rhs); }
            ActionKillEntityAtLocation& operator=(const ActionKillEntityAtLocation& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ActionKillEntityAtLocation& rhs)
            {
                ActionBase::copyFrom(rhs);
                _numberAll = rhs._numberAll;
                _entity = rhs._entity;
                _playerType = rhs._playerType;
                _location = rhs._location;
            }
            
            virtual ~ActionKillEntityAtLocation() = default;
            virtual bool drawEditMode(void* opt) override
            {
                
                ImGui::Text("Kill");
                
                // number(all) item
                ImGui::PushID(0);
                ImGui::SameLine(); _numberAll.drawImGui();
                ImGui::PopID();
                
                // entity combo box
                ImGui::PushID(1);
                ImGui::SameLine(); _entity.drawImGui();
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::Text("for");
                
                // player combo box
                ImGui::PushID(2);
                ImGui::SameLine(); _playerType.drawImGui();
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::Text("at");
                
                // location combo box
                ImGui::PushID(3);
                ImGui::SameLine(); _location.drawImGui(opt);
                ImGui::PopID();
                
                return (_numberAll.isItemSelected() &&
                        _entity.isItemSelected() &&
                        _playerType.isItemSelected() &&
                        _location.isItemSelected());
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret = "Kill ";
                ret += "'" + _numberAll.getParameterName() + "' ";
                ret += "'" + _entity.getParameterName() + "' ";
                ret += "for ";
                ret += "'" + _playerType.getParameterName() + "' ";
                ret += "at ";
                ret += "'" + _location.getParameterName() + "'";
                return ret;
            }
            
            virtual void reset() override
            {
                _numberAll.reset();
                _entity.reset();
                _playerType.reset();
                _location.reset();
            }
            
            virtual ActionKillEntityAtLocation* clone() const override { return new ActionKillEntityAtLocation(*this); }
            
            virtual flatbuffers::Offset<DeadCreator::Action> getActionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                int numberAll = (_numberAll.getNumberAll().first ? -1 /* all */ : _numberAll.getNumberAll().second);
                auto obj = DeadCreator::CreateKillEntityAtLocation(builder,
                                                                   numberAll,
                                                                   static_cast<int>(_entity.getEntityType()),
                                                                   static_cast<int>(_playerType.getPlayerType()),
                                                                   builder.CreateString(_location.getLocation()->getLocationName()));
                
                return DeadCreator::CreateAction(builder, DeadCreator::ActionBase_KillEntityAtLocation, obj.Union());
            }
            
            void setNumberAll(const std::pair<bool, int>& numberAll) { _numberAll.setNumberAll(numberAll); }
            void setEntity(EntityType type) { _entity.setEntityType(type); }
            void setPlayerType(PlayerType type) { _playerType.setPlayerType(type); }
            void setLocation(LocationNode* loc) { _location.setLocation(loc); }

        private:
            
            TriggerParameterNumberAll _numberAll;
            TriggerParameterEntity _entity;
            TriggerParameterPlayerType _playerType;
            TriggerParameterLocation _location;
            
        };
        
    }
    
    namespace client
    {
        
        struct ActionKillEntityAtLocationData: public TriggerDataBase
        {
            int number;
            EntityType entity;
            PlayerType player;
            std::string location;
            
            ActionKillEntityAtLocationData() { type = TriggerComponentType::ACTION_KILL_ENTITY_AT_LOCATION; }
        };
        
        class ActionKillEntityAtLocation : public ActionBase
        {
            
        public:
            
            explicit ActionKillEntityAtLocation(Game* game) : ActionBase(game)
            {}
            
            virtual ~ActionKillEntityAtLocation() = default;
            
            static ActionKillEntityAtLocation* create(Game* game, int number, EntityType entity, PlayerType player, const std::string& location)
            {
                auto ret = new (std::nothrow) ActionKillEntityAtLocation(game);
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
                
                const auto& entities = _game->getEntityManager()->getEntities();
                const auto& locations = _game->getGameResource()->getLocations();
                std::vector<EntityBase*> removeList;
                int numberOfRemoveEntity = 10000; /* all */
                if ( _params.number != -1 ) numberOfRemoveEntity = _params.number;
                
                for( auto& ent : entities)
                {
                    auto entity = ent.second;
                    int player = static_cast<int>(entity->getPlayerType());
                    
                    if ( _maskedPlayer.test(player) &&
                        _params.entity == entity->getEntityType() &&
                        numberOfRemoveEntity > 0 &&
                        locations.at(_params.location).intersectsCircle(entity->getWorldPosition(), entity->getBoundingRadius()) )
                    {
                        removeList.push_back(entity);
                        --numberOfRemoveEntity;
                    }
                }
                
                // kill entities
                for ( auto& ent : removeList )
                    _game->killEntity(ent);
            }
            
        private:
            
            ActionKillEntityAtLocationData _params;
            std::bitset<30> _maskedPlayer;
            
        };
        
    }
}









