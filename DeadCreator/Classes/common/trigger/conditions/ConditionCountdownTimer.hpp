//
//  ConditionCountdownTimer.h
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
        
        class GMXLayer;
        
        class ConditionCountdownTimer : public ConditionBase
        {
            
        public:
            
            explicit ConditionCountdownTimer(GMXLayer* gmxLayer) : ConditionBase(gmxLayer),
            _approximation(gmxLayer),
            _number(gmxLayer)
            {
                name() = "Countdown Timer";
            }
            ConditionCountdownTimer(const ConditionCountdownTimer& rhs) : ConditionBase(rhs) { copyFrom(rhs); }
            ConditionCountdownTimer& operator=(const ConditionCountdownTimer& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ConditionCountdownTimer& rhs)
            {
                ConditionBase::copyFrom(rhs);
                
                _approximation = rhs._approximation;
                _number = rhs._number;
            }
            
            virtual ~ConditionCountdownTimer() = default;
            virtual bool drawEditMode(void* opt) override
            {
                ImGui::Text("Countdown timer is");
                
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
                ret += "Countdown timer is ";
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
            
            virtual ConditionCountdownTimer* clone() const override
            {
                return new ConditionCountdownTimer(*this);
            }
            
            virtual flatbuffers::Offset<DeadCreator::Condition> getConditionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto obj = DeadCreator::CreateCountdownTimer(builder,
                                                          static_cast<DeadCreator::Approximation>(_approximation.getApproximationType()),
                                                          _number.getNumber());
                
                return DeadCreator::CreateCondition(builder, DeadCreator::ConditionBase_CountdownTimer, obj.Union());
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
        
        struct ConditionCountdownTimerData: public TriggerDataBase
        {
            ApproximationType approximation;
            int number;
            
            ConditionCountdownTimerData() { type = TriggerComponentType::CONDITION_COUNTDOWN_TIMER; }
        };
        
        class ConditionCountdownTimer : public ConditionBase
        {
            
        public:
            
            explicit ConditionCountdownTimer(Game* game) : ConditionBase(game)
            {
            }
            
            virtual ~ConditionCountdownTimer() = default;
            
            static ConditionCountdownTimer* create(Game* game, ApproximationType appType, int number)
            {
                auto ret = new (std::nothrow) ConditionCountdownTimer(game);
                if ( ret && ret->init(appType, number) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init(ApproximationType appType, int number)
            {
                _params.approximation = appType;
                _params.number = number;
                
                return true;
            }
            
            virtual bool isReady() override
            {
                if ( _game->isPaused() ) return false;
                
                auto countdownTimer = _game->getCountdownTimer();
                if ( countdownTimer == 0) return false;
                
                if ( _params.approximation == ApproximationType::AT_LEAST && countdownTimer >= _params.number )
                {
                    return true;
                }
                else if ( _params.approximation == ApproximationType::AT_MOST && countdownTimer <= _params.number )
                {
                    return true;
                }
                else if ( _params.approximation == ApproximationType::EXACTLY && countdownTimer >= _params.number && countdownTimer <= _params.number + 1 )
                {
                    return true;
                }
                
                return false;
            }
            
        private:
            
            ConditionCountdownTimerData _params;
            
        };
        
    }
    
}
