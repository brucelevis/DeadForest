//
//  ConditionSwitch.hpp
//  DeadCreator
//
//  Created by mac on 2016. 10. 20..
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
        
        class ConditionSwitch : public ConditionBase
        {
            
        public:
            
            explicit ConditionSwitch(GMXLayer* gmxLayer) : ConditionBase(gmxLayer),
            _name(gmxLayer),
            _status(gmxLayer)
            {
                name() = "Switch";
            }
            ConditionSwitch(const ConditionSwitch& rhs) : ConditionBase(rhs) { copyFrom(rhs); }
            ConditionSwitch& operator=(const ConditionSwitch& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ConditionSwitch& rhs)
            {
                ConditionBase::copyFrom(rhs);
                
                _name = rhs._name;
                _status = rhs._status;
            }
            
            virtual ~ConditionSwitch() = default;
            virtual bool drawEditMode(void* opt) override
            {
                // approximation combo box
                ImGui::PushID(1);
                _name.drawImGui(opt);
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::TextUnformatted("is ");
                
                // drag int
                ImGui::PushID(2);
                ImGui::SameLine(); _status.drawImGui();
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::TextUnformatted(".");
                
                return (_name.isItemSelected() && _status.isItemSelected());
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret;
                ret += "'" + _name.getParameterName() + "' ";
                ret += "is";
                ret += "'" + _status.getParameterName() + "'.";
                return ret;
            }
            
            virtual void reset() override
            {
                _name.reset();
                _status.reset();
            }
            
            virtual ConditionSwitch* clone() const override
            {
                return new ConditionSwitch(*this);
            }
            
            virtual flatbuffers::Offset<DeadCreator::Condition> getConditionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto obj = DeadCreator::CreateSwitch(builder,
                                                     builder.CreateString(_name.getSwitchName()),
                                                     _name.getSwitchIndex(),
                                                     static_cast<DeadCreator::SwitchStatus>(_status.getSwitchStatus()));
                
                return DeadCreator::CreateCondition(builder, DeadCreator::ConditionBase_Switch, obj.Union());
            }
            
            void setSwitchName(const std::string& name) { _name.setSwitchName(name); }
            void setSwitchIndex(int index) { _name.setSwitchIndex(index); }
            void setStatus(SwitchStatus status) { _status.setSwitchStatus(status); }
            
        private:
            
            TriggerParameterSwitchName _name;
            TriggerParameterSwitchStatus _status;
            
        };
        
    }
    
//    namespace client
//    {
//        
//        struct ConditionSwitchData: public TriggerDataBase
//        {
//            ApproximationType approximation;
//            int number;
//            
//            ConditionSwitchData() { type = TriggerComponentType::CONDITION_ELAPSED_TIME; }
//        };
//        
//        class ConditionSwitch : public ConditionBase
//        {
//            
//        public:
//            
//            explicit ConditionSwitch(Game* game) : ConditionBase(game)
//            {
//            }
//            
//            virtual ~ConditionSwitch() = default;
//            
//            static ConditionSwitch* create(Game* game, ApproximationType appType, int number)
//            {
//                auto ret = new (std::nothrow) ConditionSwitch(game);
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
//                auto Switch = _game->getSwitch();
//                if ( _params.approximation == ApproximationType::AT_LEAST && Switch >= _params.number )
//                {
//                    return true;
//                }
//                else if ( _params.approximation == ApproximationType::AT_MOST && Switch <= _params.number )
//                {
//                    return true;
//                }
//                else if ( _params.approximation == ApproximationType::EXACTLY && Switch >= _params.number && Switch <= _params.number + 1 )
//                {
//                    return true;
//                }
//                
//                return false;
//            }
//            
//        private:
//            
//            ConditionSwitchData _params;
//            
//        };
//        
//    }
    
}
