//
//  ActionMoveEntity.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 8. 12..
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
        
        class ActionMoveEntity : public ActionBase
        {
            
        public:
            
            explicit ActionMoveEntity(GMXLayer* gmxLayer) : ActionBase(gmxLayer),
            _numberAll(gmxLayer),
            _entity(gmxLayer),
            _playerType(gmxLayer),
            _sourceLocation(gmxLayer),
            _destLocation(gmxLayer)
            {
                name() = "Move Entity";
            }
            ActionMoveEntity(const ActionMoveEntity& rhs) : ActionBase(rhs) { copyFrom(rhs); }
            ActionMoveEntity& operator=(const ActionMoveEntity& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ActionMoveEntity& rhs)
            {
                ActionBase::copyFrom(rhs);
                _numberAll = rhs._numberAll;
                _entity = rhs._entity;
                _playerType = rhs._playerType;
                _sourceLocation = rhs._sourceLocation;
                _destLocation = rhs._destLocation;
            }
            
            virtual ~ActionMoveEntity() = default;
            virtual bool drawEditMode(void* opt) override
            {
                ImGui::Text("Move");
                
                // dest Entity combo box
                ImGui::PushID(0);
                ImGui::SameLine(); _numberAll.drawImGui(opt);
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
                
                // Entity combo box
                ImGui::PushID(3);
                ImGui::SameLine(); _sourceLocation.drawImGui(opt);
                ImGui::PopID();
                
                ImGui::Text("to");
                
                ImGui::PushID(4);
                ImGui::SameLine(); _destLocation.drawImGui(opt);
                ImGui::PopID();
                
                return (_numberAll.isItemSelected() &&
                        _entity.isItemSelected() &&
                        _playerType.isItemSelected() &&
                        _sourceLocation.isItemSelected() &&
                        _destLocation.isItemSelected());
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret = "Move ";
                ret += "'" + _numberAll.getParameterName() + "' ";
                ret += "'" + _entity.getParameterName() + "' ";
                ret += "for ";
                ret += "'" + _playerType.getParameterName() + "' ";
                ret += "at ";
                ret += "'" + _sourceLocation.getParameterName() + "' ";
                ret += "to ";
                ret += "'" + _destLocation.getParameterName() + "'.";
                return ret;
            }
            
            virtual void reset() override
            {
                _numberAll.reset();
                _entity.reset();
                _playerType.reset();
                _sourceLocation.reset();
                _destLocation.reset();
            }
            
            virtual ActionMoveEntity* clone() const override { return new ActionMoveEntity(*this); }
            
            virtual flatbuffers::Offset<DeadCreator::Action> getActionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                int numberAll = (_numberAll.getNumberAll().first ? -1 /* all */ : _numberAll.getNumberAll().second);
                auto obj = DeadCreator::CreateMoveEntity(builder,
                                                         numberAll,
                                                         static_cast<int>(_entity.getEntityType()),
                                                         static_cast<int>(_playerType.getPlayerType()),
                                                         builder.CreateString(_sourceLocation.getLocation()->getLocationName()),
                                                         builder.CreateString(_destLocation.getLocation()->getLocationName()));
                
                return DeadCreator::CreateAction(builder, DeadCreator::ActionBase_MoveEntity, obj.Union());
            }
            
            void setNumberAll(const std::pair<bool, int>& numberAll) { _numberAll.setNumberAll(numberAll); }
            void setEntity(EntityType type) { _entity.setEntityType(type); }
            void setPlayerType(PlayerType type) { _playerType.setPlayerType(type); }
            void setSourceLocation(LocationNode* loc) { _sourceLocation.setLocation(loc); }
            void setDestLocation(LocationNode* loc) { _destLocation.setLocation(loc); }
            
        private:
            
            TriggerParameterNumberAll _numberAll;
            TriggerParameterEntity _entity;
            TriggerParameterPlayerType _playerType;
            TriggerParameterLocation _sourceLocation;
            TriggerParameterLocation _destLocation;
            
        };
        
    }
    
    namespace client
    {
        
        struct ActionMoveEntityData: public TriggerDataBase
        {
            int number;
            EntityType entity;
            PlayerType player;
            std::string sourceLocation;
            std::string destLocation;
            
            ActionMoveEntityData() { type = TriggerComponentType::ACTION_MOVE_ENTITY; }
        };
        
        class ActionMoveEntity : public ActionBase
        {
            
        public:
            
            explicit ActionMoveEntity(Game* game) : ActionBase(game)
            {}
            
            virtual ~ActionMoveEntity() = default;
            
            static ActionMoveEntity* create(Game* game, int number, EntityType entity, PlayerType player, const std::string& srcLocation, const std::string& destLocation)
            {
                auto ret = new (std::nothrow) ActionMoveEntity(game);
                if ( ret && ret->init(number, entity, player, srcLocation, destLocation) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init(int number, EntityType entity, PlayerType player, const std::string& srcLocation, const std::string& destLocation)
            {
                _params.number = number;
                _params.entity = entity;
                _params.player = player;
                _params.sourceLocation = srcLocation;
                _params.destLocation = destLocation;
                
                return true;
            }
            
            virtual void doAction()
            {
                if ( _params.player == PlayerType::CURRENT_PLAYER ) _maskedPlayer = _owner->getPlayers();
                else _maskedPlayer.set(static_cast<int>(_params.player));
                
                if ( _params.number == -1 ) _params.number = 10000;
                
                int numOfMovedEntity = 0;
                const auto& entities = _game->getEntityManager()->getEntities();
                
                auto srcRect = _game->getGameResource()->getLocations().at(_params.sourceLocation);
                auto destRect = _game->getGameResource()->getLocations().at(_params.destLocation);
                
                auto srcRectCenter = cocos2d::Vec2(srcRect.getMidX(), srcRect.getMidY());
                auto destRectCenter = cocos2d::Vec2(destRect.getMidX() + cocos2d::random(-5.0f, 5.0f),
                                                    destRect.getMidY() + cocos2d::random(-5.0f, 5.0f));
                
                for ( const auto& entity : entities )
                {
                    auto currEntity = entity.second;
                    int player = static_cast<int>(currEntity->getPlayerType());
                    if ( _params.number > numOfMovedEntity && _maskedPlayer.test(player) &&
                         srcRect.intersectsCircle(currEntity->getWorldPosition(), currEntity->getBoundingRadius() * 1.1f) &&
                         currEntity->getEntityType() == _params.entity )
                    {
                        if ( isMasked(currEntity->getFamilyMask(), FamilyMask::HUMAN_BASE) )
                        {
                            if ( !static_cast<HumanBase*>(currEntity)->isAlive() )
                                continue;
                        }
                        
                        currEntity->setWorldPosition(destRectCenter);
                        numOfMovedEntity ++;
                    }
                }
            }
            
        private:
            
            ActionMoveEntityData _params;
            std::bitset<30> _maskedPlayer;
            
        };
        
    }
}









