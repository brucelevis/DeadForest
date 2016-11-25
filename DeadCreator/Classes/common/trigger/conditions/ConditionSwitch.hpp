//
//  ConditionSwitch.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 10. 20..
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
                // combo box
                ImGui::PushID(1);
                _name.drawImGui(opt);
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::TextUnformatted("is ");
                
                // status
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
                auto switchInfo = DeadCreator::CreateSwitchInfo(builder,
                                                                builder.CreateString(_name.getSwitchName()),
                                                                static_cast<DeadCreator::SwitchStatus>(_status.getSwitchStatus()),
                                                                _name.getSwitchIndex());
                
                auto obj = DeadCreator::CreateSwitch(builder, switchInfo);
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
    
    namespace client
    {
        
        struct ConditionSwitchData: public TriggerDataBase
        {
            Switch info;
            
            ConditionSwitchData() { type = TriggerComponentType::CONDITION_SWITCH; }
        };
        
        class ConditionSwitch : public ConditionBase
        {
            
        public:
            
            explicit ConditionSwitch(Game* game) : ConditionBase(game)
            {
            }
            
            virtual ~ConditionSwitch() = default;
            
            static ConditionSwitch* create(Game* game, const Switch& switchInfo)
            {
                auto ret = new (std::nothrow) ConditionSwitch(game);
                if ( ret && ret->init(switchInfo) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init(const Switch& switchInfo)
            {
                _params.info = switchInfo;
                
                return true;
            }
            
            virtual bool isReady() override
            {
                if ( _params.info.status == _game->getSwitchStatus(_params.info.index) ) return true;
                return false;
            }
            
        private:
            
            ConditionSwitchData _params;
            
        };
        
    }
    
}
