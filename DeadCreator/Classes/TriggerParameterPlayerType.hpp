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
        
        TriggerParameterPlayerType(const TriggerParameterPlayerType& rhs) : TriggerParameterBase(rhs)
        {
            _playerType = rhs._playerType;
        }
        
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
        
    private:
    
        PlayerType _playerType;
        
    };
    
}
