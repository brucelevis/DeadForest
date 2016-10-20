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
        
        class GMXLayer;
        
        class ConditionElapsedTime : public ConditionBase
        {
            
        public:
        
            explicit ConditionElapsedTime(GMXLayer* gmxLayer) : ConditionBase(gmxLayer) { name() = "Elapsed time"; }
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
                ImGui::Text("Elapsed scenario time is ");
                
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
        
        struct ConditionElapsedTimeData: public TriggerDataBase
        {
            ApproximationType approximation;
            int number;
            
            ConditionElapsedTimeData() { type = TriggerComponentType::CONDITION_ELAPSED_TIME; }
        };
        
        class ConditionElapsedTime : public ConditionBase
        {
            
        public:
            
            explicit ConditionElapsedTime(Game* game) : ConditionBase(game)
            {
            }
            
            virtual ~ConditionElapsedTime() = default;
            
            static ConditionElapsedTime* create(Game* game, ApproximationType appType, int number)
            {
                auto ret = new (std::nothrow) ConditionElapsedTime(game);
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
                auto elapsedTime = _game->getElapsedTime();
                if ( _params.approximation == ApproximationType::AT_LEAST && elapsedTime >= _params.number )
                {
                    return true;
                }
                else if ( _params.approximation == ApproximationType::AT_MOST && elapsedTime <= _params.number )
                {
                    return true;
                }
                else if ( _params.approximation == ApproximationType::EXACTLY && elapsedTime >= _params.number && elapsedTime <= _params.number + 1 )
                {
                    return true;
                }
                
                return false;
            }
            
        private:
            
            ConditionElapsedTimeData _params;
            
        };
        
    }
    
}
