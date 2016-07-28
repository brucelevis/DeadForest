//
//  ActionDisplayText.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 19..
//
//

#pragma once

#include "ActionBase.hpp"
#include "TriggerParameters.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class ActionDisplayText : public ActionBase
        {
            
        public:
            
            ActionDisplayText() { name() = "Display Text"; _text.reset(); }
            ActionDisplayText(const ActionDisplayText& rhs) : ActionBase(rhs) { copyFrom(rhs); }
            ActionDisplayText& operator=(const ActionDisplayText& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ActionDisplayText& rhs)
            {
                ActionBase::copyFrom(rhs);
                _text = rhs._text;
            }
            
            virtual ~ActionDisplayText() = default;
            virtual bool drawEditMode(void* opt) override
            {
                ImGui::BeginChild("##dummy", ImVec2(0, 250), true);
                _text.drawImGui();
                ImGui::EndChild();
                
                return _text.isItemSelected();
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret = "Display for current player. '";
                ret += _text.getParameterName();
                ret += "'";
                return ret;
            }
            
            virtual void reset() override { _text.reset(); }
            virtual ActionDisplayText* clone() const override { return new ActionDisplayText(*this); }
            virtual void deepCopy(TriggerComponentProtocol* copy) override
            {
                ActionBase::deepCopy(copy);
                ActionDisplayText* p = static_cast<ActionDisplayText*>(copy);
                _text = p->_text;
            }
            
            virtual flatbuffers::Offset<DeadCreator::Action> getActionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto obj = DeadCreator::CreateDisplayText(builder, builder.CreateString(_text.getText()));
                return DeadCreator::CreateAction(builder, DeadCreator::ActionBase_DisplayText, obj.Union());
            }
            
            void setText(const std::string& text) { _text.setText(text); }
            
        private:
            
            TriggerParameterText _text;
            
        };
        
    }
}









