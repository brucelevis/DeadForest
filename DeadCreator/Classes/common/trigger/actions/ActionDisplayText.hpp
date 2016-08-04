//
//  ActionDisplayText.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 19..
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
                ImGui::Text("Display for current player.");
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
    
    namespace client
    {
        
        struct ActionDisplayTextData: public TriggerDataBase
        {
            std::string text;
            
            ActionDisplayTextData() { type = TriggerComponentType::ACTION_DISPLAY_TEXT; }
        };
        
        class ActionDisplayText : public ActionBase
        {
            
        public:
            
            explicit ActionDisplayText(Game* game) : ActionBase(game)
            {}
            
            virtual ~ActionDisplayText() = default;
            
            static ActionDisplayText* create(Game* game, const std::string& text)
            {
                auto ret = new (std::nothrow) ActionDisplayText(game);
                if ( ret && ret->init(text) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init(const std::string& text)
            {
                _params.text = text;
                return true;
            }
            
            virtual void doAction()
            {
                _game->addLog(cocos2d::StringUtils::format("display text: \"%s\"", _params.text.c_str()));
            }
            
        private:
            
            ActionDisplayTextData _params;
            std::bitset<9> _maskedPlayer;
            
        };
        
    }
}









