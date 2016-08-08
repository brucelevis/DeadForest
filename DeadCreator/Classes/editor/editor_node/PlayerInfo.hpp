//
//  PlayerInfo.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 7..
//
//

#pragma once

#include "Types.hpp"

namespace realtrick
{
    namespace editor
    {
        
        enum class Force : int { FORCE_1 = 0, FORCE_2 = 1 };
        enum class Owner : int { HUMAN = 0, COMPUTER = 1, UNUSED = 2 };
        
        struct PlayerInfo
        {
            PlayerType player;
            Force force;
            Owner owner;
            
            PlayerInfo(PlayerType p, Force f, Owner o) :
            player(p),
            force(f),
            owner(o)
            {}
        };
        
    }
}