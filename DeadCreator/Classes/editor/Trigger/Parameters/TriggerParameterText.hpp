//
//  TriggerParameterText.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 26..
//
//

#pragma once

#include <array>

#include "TriggerParameterBase.hpp"

#define MAX_TEXT_LENGTH 32

namespace realtrick
{
    
    class TriggerParameterText : public TriggerParameterBase
    {
        
    public:
        
        TriggerParameterText() : TriggerParameterBase()
        {
            _parameterType = TriggerParameterBase::Type::TEXT;
            setParameterName("");
        }
        
        TriggerParameterText(const TriggerParameterText& rhs) { copyFrom(rhs); }
        TriggerParameterText& operator=(const TriggerParameterText& rhs)
        {
            if ( &rhs != this ) copyFrom(rhs);
            return *this;
        }
        
        void copyFrom(const TriggerParameterText& rhs)
        {
            TriggerParameterBase::copyFrom(rhs);
            strncpy(_buf.data(), rhs._buf.data(), MAX_TEXT_LENGTH);
        }
        
        virtual ~TriggerParameterText() = default;
        
        std::string getText() const { return _buf.data(); }
        void setText(const std::string& text)
        {
            strncpy(_buf.data(), text.c_str(), MAX_TEXT_LENGTH);
            setParameterName(_buf.data());
        }
        
        virtual TriggerParameterText* clone() const override
        {
            return new TriggerParameterText(*this);
        }
        
        virtual void reset() override { _buf.front() = '\0'; }
        virtual bool isItemSelected() override { return (_buf.front() != '\0'); }
        
        virtual void drawImGui(void* opt = nullptr) override
        {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(ImVec4(0.85, 0.85, 0.85, 1.00)));
            ImGui::Text("Display for current player.");
            if (ImGui::InputText("", _buf.data(), MAX_TEXT_LENGTH))
            {
                setParameterName(_buf.data());
            }
            ImGui::PopStyleColor();
        }
        
    private:
        
        std::array<char, MAX_TEXT_LENGTH> _buf;
        
    };
    
}









