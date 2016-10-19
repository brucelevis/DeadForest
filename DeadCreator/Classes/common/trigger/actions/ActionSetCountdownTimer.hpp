//
//  ActionSetCountdownTimer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 9. 5..
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
        
        class ActionSetCountdownTimer : public ActionBase
        {
            
        public:
            
            ActionSetCountdownTimer() { name() = "Set Countdown Timer"; }
            ActionSetCountdownTimer(const ActionSetCountdownTimer& rhs) : ActionBase(rhs) { copyFrom(rhs); }
            ActionSetCountdownTimer& operator=(const ActionSetCountdownTimer& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const ActionSetCountdownTimer& rhs)
            {
                ActionBase::copyFrom(rhs);
                _arithmetical = rhs._arithmetical;
                _number = rhs._number;
            }
            
            virtual ~ActionSetCountdownTimer() = default;
            virtual bool drawEditMode(void* opt) override
            {
                ImGui::TextUnformatted("Modify Countdown Timer:");
                
                // arithmetical combo box
                ImGui::SameLine(); _arithmetical.drawImGui();
                
                // number
                ImGui::SameLine(); _number.drawImGui();
                
                ImGui::SameLine(); ImGui::TextUnformatted("seconds.");
                
                return true;
            }
            
            virtual std::string getSummaryString() const override
            {
                std::string ret = "Modify Countdown Timer: ";
                ret += "'" + _arithmetical.getParameterName() + "' ";
                ret += "'" + _number.getParameterName() + "' ";
                ret += "seconds.";
                return ret;
            }
            
            virtual void reset() override { }
            virtual ActionSetCountdownTimer* clone() const override { return new ActionSetCountdownTimer(*this); }
            
            virtual flatbuffers::Offset<DeadCreator::Action> getActionObject(flatbuffers::FlatBufferBuilder& builder) override
            {
                auto obj = DeadCreator::CreateSetCountdownTimer(builder,
                                                                static_cast<DeadCreator::Arithmetical>(_arithmetical.getArithmeticalType()),
                                                                _number.getNumber());
                return DeadCreator::CreateAction(builder, DeadCreator::ActionBase_SetCountdownTimer, obj.Union());
            }
            
            void setArithmeticalType(ArithmeticalType type) { _arithmetical.setArithmeticalType(type); }
            void setNumber(int number) { _number.setNumber(number); }
            
        private:
            
            TriggerParameterArithmetical _arithmetical;
            TriggerParameterNumber _number;
            
        };
        
    }
    
    namespace client
    {
        
        struct ActionSetCountdownTimerData: public TriggerDataBase
        {
            ArithmeticalType arithmetical;
            int number;
            
            ActionSetCountdownTimerData() { type = TriggerComponentType::ACTION_SET_COUNTDOWN_TIMER; }
        };
        
        class ActionSetCountdownTimer : public ActionBase
        {
            
        public:
            
            explicit ActionSetCountdownTimer(Game* game) : ActionBase(game)
            {}
            
            virtual ~ActionSetCountdownTimer() = default;
            
            static ActionSetCountdownTimer* create(Game* game, ArithmeticalType arithmeticalType, int number)
            {
                auto ret = new (std::nothrow) ActionSetCountdownTimer(game);
                if ( ret && ret->init(arithmeticalType, number) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init(ArithmeticalType arithmeticalType, int number)
            {
                _params.arithmetical = arithmeticalType;
                _params.number = number;
                
                return true;
            }
            
            virtual void doAction()
            {
                auto number = _params.number;
                if ( _params.arithmetical == ArithmeticalType::SET_TO)
                {
                    _game->setCountdownTimer(number);
                }
                else if ( _params.arithmetical == ArithmeticalType::ADD)
                {
                    _game->addCountdownTimer(number);
                }
                else if ( _params.arithmetical == ArithmeticalType::SUBTRACT)
                {
                    _game->subtractCountdownTimer(number);
                }
            }
            
        private:
            
            ActionSetCountdownTimerData _params;
            
        };
        
    }
}









