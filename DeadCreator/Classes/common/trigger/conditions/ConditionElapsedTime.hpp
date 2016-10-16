//
//  ConditionElapsedTime.h
//  DeadCreator
//
//  Created by mac on 2016. 10. 14..
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
        
        class ConditionElapsedTime : public ConditionBase
        {
            
        public:
            
            ConditionElapsedTime() { name() = "Elapsed time"; }
            ConditionElapsedTime(const ConditionElapsedTime& rhs) : ConditionBase(rhs) { copyFrom(rhs); }
            ConditionElapsedTime& operator=(const ConditionElapsedTime& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ConditionElapsedTime& rhs)
            {
                ConditionBase::copyFrom(rhs);
                
                _approximation = rhs._approximation;
                _number = rhs._number;
            }
            
            virtual ~ConditionElapsedTime() = default;
            virtual bool drawEditMode(void* opt) override
            {
                ImGui::SameLine(); ImGui::Text("Elapsed scenario time is ");
                
                // approximation combo box
                ImGui::PushID(1);
                ImGui::SameLine(); _approximation.drawImGui();
                ImGui::PopID();
                
                // drag int
                ImGui::PushID(2);
                ImGui::SameLine(); _number.drawImGui();
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::Text("game seconds.");
                
                return (_approximation.isItemSelected());
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret;
                ret += "Elapsed scenario time is ";
                ret += "'" + _approximation.getParameterName() + "' ";
                ret += "'" + _number.getParameterName() + "' ";
                ret += "game seconds.";
                return ret;
            }
            
            virtual void reset() override
            {
                _approximation.reset();
                _number.reset();
            }
            
            virtual ConditionElapsedTime* clone() const override
            {
                return new ConditionElapsedTime(*this);
            }
            
            virtual flatbuffers::Offset<DeadCreator::Condition> getConditionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto obj = DeadCreator::CreateElapsedTime(builder,
                                                          static_cast<DeadCreator::Approximation>(_approximation.getApproximationType()),
                                                          _number.getNumber());
                
                return DeadCreator::CreateCondition(builder, DeadCreator::ConditionBase_ElapsedTime, obj.Union());
            }
            
            void setApproximation(ApproximationType type) { _approximation.setApproximationType(type); }
            void setNumber(int number) { _number.setNumber(number); }
            
        private:
            
            TriggerParameterApproximation _approximation;
            TriggerParameterNumber _number;
            
        };
        
    }
    
    namespace client
    {
        
//        struct ConditionElapsedTimeData: public TriggerDataBase
//        {
//            ApproximationType approximation;
//            int number;
//            
//            ConditionElapsedTimeData() { type = TriggerComponentType::CONDITION_ELAPSED_TIME; }
//        };
//        
//        class ConditionElapsedTime : public ConditionBase
//        {
//            
//        public:
//            
//            explicit ConditionElapsedTime(Game* game) : ConditionBase(game)
//            {
//            }
//            
//            virtual ~ConditionElapsedTime() = default;
//            
//            static ConditionElapsedTime* create(Game* game, ApproximationType appType, int number)
//            {
//                auto ret = new (std::nothrow) ConditionElapsedTime(game);
//                if ( ret && ret->init(appType, number) )
//                {
//                    ret->autorelease();
//                    return ret;
//                }
//                CC_SAFE_DELETE(ret);
//                return nullptr;
//            }
//            
//            bool init(ApproximationType appType, int number)
//            {
//                _params.approximation = appType;
//                _params.number = number;
//                
//                return true;
//            }
//            
//            virtual bool isReady() override
//            {
//                if ( _params.player == PlayerType::CURRENT_PLAYER ) _maskedPlayer = _owner->getPlayers();
//                    else _maskedPlayer.set(static_cast<int>(_params.player));
//                        
//                        const auto& entities = _game->getEntityManager()->getEntities();
//                        const auto& locations = _game->getGameResource()->getLocations();
//                        
//                        // 플레이어 타입인지 체크. (o)
//                        // 엔티티 타입 체크. (o)
//                        // 충돌됐는지 체크. (o)
//                        // 숫자맞는지 체크. (x)
//                        int numberOfReadyEntities = 0;
//                for( auto& ent : entities)
//                {
//                    auto entity = ent.second;
//                    int player = static_cast<int>(entity->getPlayerType());
//                    
//                    if ( _maskedPlayer.test(player) && _params.entity == entity->getEntityType())
//                    {
//                        bool intersect = false;
//                        if ( isMasked(ent.second->getFamilyMask(), FamilyMask::HUMAN_BASE) )
//                        {
//                            HumanBase* human = static_cast<HumanBase*>(ent.second);
//                            intersect = (human->isAlive() &&
//                                         locations.at(_params.location).intersectsCircle(entity->getWorldPosition(), entity->getBoundingRadius()));
//                            
//                        }
//                        else
//                        {
//                            intersect = locations.at(_params.location).intersectsCircle(entity->getWorldPosition(), entity->getBoundingRadius());
//                        }
//                        
//                        if ( intersect )
//                        {
//                            numberOfReadyEntities++;
//                        }
//                    }
//                }
//                
//                if ( _params.approximation == ApproximationType::AT_LEAST && numberOfReadyEntities >= _params.number ) return true;
//                else if ( _params.approximation == ApproximationType::AT_MOST && numberOfReadyEntities <= _params.number ) return true;
//                else if ( _params.approximation == ApproximationType::EXACTLY && numberOfReadyEntities == _params.number ) return true;
//                
//                return false;
//            }
//
//        private:
//            
//            ConditionElapsedTimeData _params;
//            std::bitset<30> _maskedPlayer;
//            
//        };
        
    }
    
}
