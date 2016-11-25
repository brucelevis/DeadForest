//
//  ActionSetSwitch.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 10. 21..
//
//

#pragma once

#include "ActionBase.hpp"
#include "TriggerParameters.hpp"
#include "Game.hpp"


namespace realtrick
{
    namespace editor
    {
        
        class GMXLayer;
        
        class ActionSetSwitch : public ActionBase
        {
            
        public:
            
            explicit ActionSetSwitch(GMXLayer* gmxLayer) : ActionBase(gmxLayer),
            _status(gmxLayer),
            _name(gmxLayer)
            {
                name() = "Set Switch";
            }
            ActionSetSwitch(const ActionSetSwitch& rhs) : ActionBase(rhs) { copyFrom(rhs); }
            ActionSetSwitch& operator=(const ActionSetSwitch& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ActionSetSwitch& rhs)
            {
                ActionBase::copyFrom(rhs);
                _status = rhs._status;
                _name = rhs._name;
            }
            
            virtual ~ActionSetSwitch() = default;
            virtual bool drawEditMode(void* opt) override
            {
                // draw imgui
                ImGui::PushID(0);
                _status.drawImGui();
                ImGui::PopID();
                
                ImGui::PushID(1);
                ImGui::SameLine(); _name.drawImGui();
                ImGui::PopID();
                
                ImGui::SameLine(); ImGui::TextUnformatted(".");
                
                return (_status.isItemSelected() && _name.isItemSelected());
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret;
                ret += "'" + _status.getParameterName() + "' ";
                ret += "'" + _name.getParameterName() + "'.";
                
                return ret;
            }
            
            virtual void reset() override { }
            virtual ActionSetSwitch* clone() const override { return new ActionSetSwitch(*this); }
            
            virtual flatbuffers::Offset<DeadCreator::Action> getActionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto switchInfo = DeadCreator::CreateSwitchInfo2(builder,
                                                                 builder.CreateString(_name.getSwitchName()),
                                                                 static_cast<DeadCreator::SwitchStatus2>(_status.getSwitchStatus()),
                                                                 _name.getSwitchIndex());
                
                auto obj = DeadCreator::CreateSetSwitch(builder, switchInfo);
                return DeadCreator::CreateAction(builder, DeadCreator::ActionBase_SetSwitch, obj.Union());
            }
    
            void setStatus(SwitchStatus2 status) { _status.setSwitchStatus(status); }
            void setSwitchName(const std::string& name) { _name.setSwitchName(name); }
            void setSwitchIndex(int index) { _name.setSwitchIndex(index); }
            
        private:
            
            TriggerParameterSwitchStatus2 _status;
            TriggerParameterSwitchName _name;
            
        };
        
    }
    
    namespace client
    {
        
        struct ActionSetSwitchData: public TriggerDataBase
        {
            Switch2 info;
            
            ActionSetSwitchData() { type = TriggerComponentType::ACTION_SET_SWITCH; }
        };
        
        class ActionSetSwitch : public ActionBase
        {
            
        public:
            
            explicit ActionSetSwitch(Game* game) : ActionBase(game)
            {}
            
            virtual ~ActionSetSwitch() = default;
            
            static ActionSetSwitch* create(Game* game, const Switch2& info)
            {
                auto ret = new (std::nothrow) ActionSetSwitch(game);
                if ( ret && ret->init(info) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init(const Switch2& info )
            {
                _params.info = info;
                
                return true;
            }
            
            virtual void doAction()
            {
                SwitchStatus status = SwitchStatus::INVALID;
                
                if ( _params.info.status == SwitchStatus2::RANDOMIZE )
                {
                    auto rand = cocos2d::random(0, 1);
                    if ( rand == 0 ) status = SwitchStatus::SET;
                    else status = SwitchStatus::CLEARED;
                }
                else if ( _params.info.status == SwitchStatus2::TOGGLE )
                {
                    auto oldStatus = _game->getSwitchStatus(_params.info.index);
                    if ( oldStatus == SwitchStatus::SET ) status = SwitchStatus::CLEARED;
                    else if ( oldStatus == SwitchStatus::CLEARED ) status = SwitchStatus::SET;
                    else CCASSERT(false, "invalid status");
                }
                else if ( _params.info.status == SwitchStatus2::SET ) status = SwitchStatus::SET;
                else if ( _params.info.status == SwitchStatus2::CLEAR ) status = SwitchStatus::CLEARED;
                else CCASSERT(false, "invalid switch status");
                
                if ( status != SwitchStatus::INVALID) _game->setSwitchStatus(_params.info.index, status);
                else CCASSERT(false, "invalid switch status");
            }
            
        private:
            
            ActionSetSwitchData _params;
            
        };
        
    }
}
