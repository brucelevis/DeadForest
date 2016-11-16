//
//  ConditionCommand.hpp
//  DeadCreator
//
//  Created by mac on 2016. 11. 15..
//
//


#pragma once


#include "TriggerParameters.hpp"
#include "EditorEntity.hpp"
#include "GMXLayer.hpp"
#include "GameTrigger.hpp"
#include "EntityManager.hpp"

#include "Game.hpp"
#include "GameResource.hpp"
#include "EntityBase.hpp"
#include "Types.hpp"


namespace realtrick
{
    namespace editor
    {
        
        class GMXLayer;
        
        class ConditionCommand : public ConditionBase
        {
            
        public:
            
            explicit ConditionCommand(GMXLayer* gmxLayer) : ConditionBase(gmxLayer),
            _playerType(gmxLayer),
            _approximation(gmxLayer),
            _number(gmxLayer),
            _entity(gmxLayer)
            {
                name() = "Command";
            }
            ConditionCommand(const ConditionCommand& rhs) : ConditionBase(rhs) { copyFrom(rhs); }
            ConditionCommand& operator=(const ConditionCommand& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ConditionCommand& rhs)
            {
                ConditionBase::copyFrom(rhs);
                
                _playerType = rhs._playerType;
                _approximation = rhs._approximation;
                _number = rhs._number;
                _entity = rhs._entity;
            }
            
            virtual ~ConditionCommand() = default;
            virtual bool drawEditMode(void* opt) override
            {
                // player combo box
                ImGui::PushID(0);
                _playerType.drawImGui();
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::Text("commands");
                
                // approximation combo box
                ImGui::PushID(1);
                ImGui::SameLine(); _approximation.drawImGui();
                ImGui::PopID();
                
                // drag int
                ImGui::PushID(2);
                ImGui::SameLine(); _number.drawImGui();
                ImGui::PopID();
                
                // entity combo box
                ImGui::PushID(3);
                ImGui::SameLine(); _entity.drawImGui();
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::Text(".");
                
                return (_playerType.isItemSelected() &&
                        _approximation.isItemSelected() &&
                        _entity.isItemSelected());
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret;
                ret += "'" + _playerType.getParameterName() + "' ";
                ret += "commands ";
                ret += "'" + _approximation.getParameterName() + "' ";
                ret += "'" + _number.getParameterName() + "' ";
                ret += "'" + _entity.getParameterName() + "'.";
                return ret;
            }
            
            virtual void reset() override
            {
                _playerType.reset();
                _approximation.reset();
                _number.reset();
                _entity.reset();
            }
            
            virtual ConditionCommand* clone() const override
            {
                return new ConditionCommand(*this);
            }
            
            virtual flatbuffers::Offset<DeadCreator::Condition> getConditionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto obj = DeadCreator::CreateCommand(builder,
                                                      static_cast<int>(_playerType.getPlayerType()),
                                                      static_cast<DeadCreator::Approximation>(_approximation.getApproximationType()),
                                                      _number.getNumber(),
                                                      _entity.getEntityType());
                
                return DeadCreator::CreateCondition(builder, DeadCreator::ConditionBase_Command, obj.Union());
            }
            
            void setPlayerType(PlayerType type) { _playerType.setPlayerType(type); }
            void setApproximation(ApproximationType type) { _approximation.setApproximationType(type); }
            void setNumber(int number) { _number.setNumber(number); }
            void setEntity(EntityType type) { _entity.setEntityType(type); }
            
        private:
            
            TriggerParameterPlayerType _playerType;
            TriggerParameterApproximation _approximation;
            TriggerParameterNumber _number;
            TriggerParameterEntity _entity;
            
        };
        
    }
    
    namespace client
    {
        
        struct ConditionCommandData: public TriggerDataBase
        {
            PlayerType player;
            ApproximationType approximation;
            int number;
            EntityType entity;
            
            ConditionCommandData() { type = TriggerComponentType::CONDITION_COMMAND; }
        };
        
        class ConditionCommand : public ConditionBase
        {
            
        public:
            
            explicit ConditionCommand(Game* game) : ConditionBase(game)
            {
            }
            
            virtual ~ConditionCommand() = default;
            
            static ConditionCommand* create(Game* game, PlayerType playerType, ApproximationType appType, int number, EntityType entType)
            {
                auto ret = new (std::nothrow) ConditionCommand(game);
                if ( ret && ret->init(playerType, appType, number, entType) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init(PlayerType playerType, ApproximationType appType, int number, EntityType entType)
            {
                _params.player = playerType;
                _params.approximation = appType;
                _params.number = number;
                _params.entity = entType;
                
                return true;
            }
            
            virtual bool isReady() override
            {
                if ( _game->isPaused() ) return false;
                
                if ( _params.player == PlayerType::CURRENT_PLAYER )
                {
                    _maskedPlayer = _owner->getPlayers();
                }
                else
                {
                    _maskedPlayer.set(static_cast<int>(_params.player));
                }
                
                int numberOfCommandedEntities = 0;
    
                const auto& entities = _game->getEntityManager()->getEntities();
                for( auto& ent : entities)
                {
                    auto entity = ent.second;
                    if ( !_maskedPlayer.test(static_cast<int>(entity->getPlayerType())) ) continue;
                    if ( entity->getPlayerType() != _params.player ) continue;
                    if ( entity->getEntityType() != _params.entity ) continue;
                    if ( isMasked(entity->getFamilyMask(), FamilyMask::HUMAN_BASE) )
                    {
                        if ( !static_cast<HumanBase*>(entity)->isAlive() ) continue;
                    }
                    
                    numberOfCommandedEntities++;
                }
                
                if ( _params.approximation == ApproximationType::AT_LEAST && numberOfCommandedEntities >= _params.number ) return true;
                else if ( _params.approximation == ApproximationType::AT_MOST && numberOfCommandedEntities <= _params.number ) return true;
                else if ( _params.approximation == ApproximationType::EXACTLY && numberOfCommandedEntities == _params.number ) return true;
                
                return false;
            }
            
        private:
            
            ConditionCommandData _params;
            std::bitset<30> _maskedPlayer;
            
        };
        
    }
    
}
