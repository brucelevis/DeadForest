//
//  TriggerParameterOrderType.hp
//  DeadCreator
//
//  Created by mac on 2016. 11. 14..
//
//

#pragma once

#include "TriggerParameterBase.hpp"
#include "GMXLayer.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class GMXLayer;
        
        class TriggerParameterOrderType : public TriggerParameterBase
        {
            
        public:
            
            TriggerParameterOrderType() = default;
            explicit TriggerParameterOrderType(GMXLayer* layer) : TriggerParameterBase(layer),
            _orderType(OrderType::MOVE)
            {
                setParameterName("#invalid");
            }
            
            TriggerParameterOrderType(const TriggerParameterOrderType& rhs) : TriggerParameterBase(rhs) { copyFrom(rhs); }
            TriggerParameterOrderType& operator=(const TriggerParameterOrderType& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const TriggerParameterOrderType& rhs)
            {
                TriggerParameterBase::copyFrom(rhs);
                _orderType = rhs._orderType;
                _currOrder = rhs._currOrder;
            }
            
            virtual ~TriggerParameterOrderType() {}
            
            OrderType getOrderType() const { return _orderType; }
            void setOrderType(OrderType type)
            {
                if ( type == OrderType::MOVE ) setParameterName("move");
                else if ( type == OrderType::ATTACK ) setParameterName("attack");
                
                _orderType = type;
                _currOrder = static_cast<int>(type);
            }
            
            virtual TriggerParameterOrderType* clone() const override
            {
                return new TriggerParameterOrderType(*this);
            }
            
            virtual void reset() override { _currOrder = -1; }
            virtual bool isItemSelected() override { return (_currOrder != -1); }
            
            virtual void drawImGui(void* opt = nullptr) override
            {
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85, 0.85, 0.85, 1.0));
                ImGui::PushItemWidth(150);
                static const char* items2[2] =
                {
                    "move",
                    "attack"
                };
                if (ImGui::Combo("", &_currOrder, items2, 2, 2))
                {
                    setOrderType(static_cast<OrderType>(_currOrder));
                }
                ImGui::PopItemWidth();
                ImGui::PopStyleColor();
            }
            
        private:
            
            OrderType _orderType;
            int _currOrder = -1;
            
        };
        
    }
}
