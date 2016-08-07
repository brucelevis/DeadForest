//
//  PlayerInfo.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 7..
//
//

#include "Types.hpp"

namespace realtrick
{
    namespace editor
    {
        
        enum class Force : int { FORCE_1, FORCE_2 };
        enum class Owner : int { HUMAN, COMPUTER, UNUSED };
        
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