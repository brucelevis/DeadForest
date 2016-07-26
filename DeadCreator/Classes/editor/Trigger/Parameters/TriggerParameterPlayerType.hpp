//
//  TriggerParameterPlayerType.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 18..
//
//

#pragma once

#include "TriggerParameterBase.hpp"
#include "EntityType.hpp"

namespace realtrick
{
    
    class TriggerParameterPlayerType : public TriggerParameterBase
    {
    
    public:
        
        TriggerParameterPlayerType() : TriggerParameterBase(),
        _playerType(PlayerType::INVALID)
        {
            _parameterType = Type::PLAYER;
            setParameterName("#invalid");
        }
        
        TriggerParameterPlayerType(const TriggerParameterPlayerType& rhs) { copyFrom(rhs); }
        TriggerParameterPlayerType& operator=(const TriggerParameterPlayerType& rhs)
        {
            if ( &rhs != this ) copyFrom(rhs);
            return *this;
        }
        
        void copyFrom(const TriggerParameterPlayerType& rhs)
        {
            TriggerParameterBase::copyFrom(rhs);
            _playerType = rhs._playerType;
            _currPlayer = rhs._currPlayer;
        }
        
        virtual ~TriggerParameterPlayerType() = default;
        
        PlayerType getPlayerType() const { return _playerType; }
        void setPlayerType(PlayerType type)
        {
            _playerType = type;
            if ( type == PlayerType::PLAYER1 ) setParameterName("Player 1");
            else if ( type == PlayerType::PLAYER2 ) setParameterName("Player 2");
            else if ( type == PlayerType::PLAYER3 ) setParameterName("Player 3");
            else if ( type == PlayerType::PLAYER4 ) setParameterName("Player 4");
            else if ( type == PlayerType::PLAYER5 ) setParameterName("Player 5");
            else if ( type == PlayerType::PLAYER6 ) setParameterName("Player 6");
            else if ( type == PlayerType::PLAYER7 ) setParameterName("Player 7");
            else if ( type == PlayerType::PLAYER8 ) setParameterName("Player 8");
            else if ( type == PlayerType::CURRENT_PLAYER ) setParameterName("Current Player");
        }
        
        virtual TriggerParameterPlayerType* clone() const override
        {
            return new TriggerParameterPlayerType(*this);
        }
        
        virtual void reset() override { _currPlayer = -1; }
        virtual bool isItemSelected() override { return (_currPlayer != -1); }
        
        virtual void drawImGui(void* opt = nullptr) override
        {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85, 0.85, 0.85, 1.0));
            ImGui::PushItemWidth(180);
            static const char* items1[9] =
            {
                "Player 1",
                "Player 2",
                "Player 3",
                "Player 4",
                "Player 5",
                "Player 6",
                "Player 7",
                "Player 8",
                "Current Player"
            };
            
            if ( ImGui::Combo("##", &_currPlayer, items1, 9, 9) )
            {
                setPlayerType(static_cast<PlayerType>(_currPlayer + 1));
            }
            ImGui::PopItemWidth();
            ImGui::PopStyleColor();
        }
        
    private:
    
        PlayerType _playerType;
        int _currPlayer = -1;
        
    };
    
}









