//
//  ConditionBring.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 18..
//
//

#pragma once

#include "TriggerParameters.hpp"
#include "EditorEntity.hpp"
#include "GMXLayer.hpp"
#include "GameTrigger.hpp"

#include "GameManager.hpp"
#include "EntityBase.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class ConditionBring : public ConditionBase
        {
            
        public:
            
            ConditionBring() { name() = "Bring"; }
            ConditionBring(const ConditionBring& rhs) : ConditionBase(rhs) { copyFrom(rhs); }
            ConditionBring& operator=(const ConditionBring& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ConditionBring& rhs)
            {
                ConditionBase::copyFrom(rhs);
                
                _playerType = rhs._playerType;
                _approximation = rhs._approximation;
                _number = rhs._number;
                _entity = rhs._entity;
                _location = rhs._location;
            }
            
            virtual ~ConditionBring() = default;
            virtual bool drawEditMode(void* opt) override
            {
                ImGui::BeginChild("##dummy", ImVec2(0, 250), true);
                
                // player combo box
                ImGui::PushID(0);
                _playerType.drawImGui();
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::Text("brings");
                
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
                
                ImGui::SameLine(); ImGui::Text("to");
                
                // location combo box
                ImGui::PushID(4);
                _location.drawImGui(opt);
                ImGui::PopID();
                
                ImGui::EndChild();
                
                return (_playerType.isItemSelected() &&
                        _approximation.isItemSelected() &&
                        _entity.isItemSelected() &&
                        _location.isItemSelected());
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret;
                ret += "'" + _playerType.getParameterName() + "' ";
                ret += "brings ";
                ret += "'" + _approximation.getParameterName() + "' ";
                ret += "'" + _number.getParameterName() + "' ";
                ret += "'" + _entity.getParameterName() + "' ";
                ret += "to ";
                ret += "'" + _location.getParameterName() + "'";
                return ret;
            }
            
            virtual void reset() override
            {
                _playerType.reset();
                _approximation.reset();
                _number.reset();
                _entity.reset();
                _location.reset();
            }
            
            virtual ConditionBring* clone() const override
            {
                return new ConditionBring(*this);
            }
            
            virtual void deepCopy(TriggerComponentProtocol* copy) override
            {
                ConditionBase::deepCopy(copy);
                
                auto p = static_cast<ConditionBring*>(copy);
                _playerType = p->_playerType;
                _approximation = p->_approximation;
                _number = p->_number;
                _entity = p->_entity;
                _location = p->_location;
            }
            
            virtual flatbuffers::Offset<DeadCreator::Condition> getConditionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto locationPtr = _location.getLocation();
                auto obj = DeadCreator::CreateBring(builder,
                                                    static_cast<int>(_playerType.getPlayerType()),
                                                    static_cast<DeadCreator::Approximation>(_approximation.getApproximationType()),
                                                    _number.getNumber(),
                                                    _entity.getEntityType(),
                                                    builder.CreateString(locationPtr->getLocationName()));
                
                return DeadCreator::CreateCondition(builder, DeadCreator::ConditionBase_Bring, obj.Union());
            }
            
            void setPlayerType(PlayerType type) { _playerType.setPlayerType(type); }
            void setApproximation(ApproximationType type) { _approximation.setApproximationType(type); }
            void setNumber(int number) { _number.setNumber(number); }
            void setEntity(EntityType type) { _entity.setEntityType(type); }
            void setLocation(LocationNode* loc) { _location.setLocation(loc); }
            
        private:
            
            TriggerParameterPlayerType _playerType;
            TriggerParameterApproximation _approximation;
            TriggerParameterNumber _number;
            TriggerParameterEntity _entity;
            TriggerParameterLocation _location;
            
        };
        
    }
    
    namespace client
    {
        
        class ConditionBring : public ConditionBase
        {
            
        public:
            
            explicit ConditionBring(GameManager* mgr) : ConditionBase(mgr)
            {}
            
            virtual ~ConditionBring() = default;
            
            static ConditionBring* create(GameManager* mgr, PlayerType playerType, ApproximationType appType, int number, EntityType entType, const cocos2d::Rect& loc)
            {
                auto ret = new (std::nothrow) ConditionBring(mgr);
                if ( ret && ret->init(playerType, appType, number, entType, loc) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init(PlayerType playerType, ApproximationType appType, int number, EntityType entType, const cocos2d::Rect& loc)
            {
                _player = playerType;
                _approximation = appType;
                _number = number;
                _entity = entType;
                _location = loc;
                
                return true;
            }
            
            virtual bool isReady() override
            {
                
                if ( _player == PlayerType::CURRENT_PLAYER ) _maskedPlayer = _owner->getPlayers();
                else _maskedPlayer.set(static_cast<int>(_player));
                
                const auto& entities = _gameMgr->getEntities();
                
                // 플레이어 타입인지 체크. (o)
                // 엔티티 타입 체크. (o)
                // 충돌됐는지 체크. (o)
                // 숫자맞는지 체크. (x)
                int numberOfReadyEntities = 0;
                for( auto& ent : entities)
                {
                    auto entity = ent.second;
                    int player = static_cast<int>(entity->getPlayerType());
                    
                    if (_maskedPlayer[player] &&
                        _entity == entity->getEntityType() &&
                        _location.intersectsRect(cocos2d::Rect(entity->getWorldPosition() - cocos2d::Vec2(15, 15), cocos2d::Size(30, 30))) )
                    {
                        numberOfReadyEntities++;
                    }
                }
                
                if ( _approximation == ApproximationType::AT_LEAST && numberOfReadyEntities >= _number )
                {
                    cocos2d::log("condition bring isReady() called. (at least)");
                    return true;
                }
                else if ( _approximation == ApproximationType::AT_MOST && numberOfReadyEntities <= _number )
                {
                    cocos2d::log("condition bring isReady() called. (at most)");
                    return true;
                }
                else if ( _approximation == ApproximationType::EXACTLY && numberOfReadyEntities == _number )
                {
                    cocos2d::log("condition bring isReady() called. (exactly)");
                    return true;
                }
                
                return false;
            }
            
        private:
            
            PlayerType _player;
            ApproximationType _approximation;
            int _number;
            EntityType _entity;
            cocos2d::Rect _location;
            
            std::bitset<9> _maskedPlayer;
            
        };
        
    }
    
}









