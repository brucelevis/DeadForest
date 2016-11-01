//
//  Infos.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 8. 7..
//
//

#pragma once

#include <array>

#include "Types.hpp"

namespace realtrick
{
    
    enum class Force : int { FORCE_1 = 0, FORCE_2 = 1, FORCE_3 = 2, FORCE_4 = 3 };
    enum class Owner : int { HUMAN = 0, COMPUTER = 1, UNUSED = 2 };
            
    struct PlayerInfo
    {
        PlayerType player;
        Force force;
        Owner owner;
        bool isAllyWith[9];
        bool isSharedVision[9];
        
        PlayerInfo(PlayerType p = PlayerType::INVALID, Force f = Force::FORCE_1, Owner o = Owner::UNUSED) :
        player(p),
        force(f),
        owner(o)
        {
            for(auto i = 0 ; i < 9 ; ++ i) isAllyWith[i] = isSharedVision[i] = false;
        }
    };
            
    struct ForceInfo
    {
        std::array<char, 20> name;
        bool isAlly;
        bool isVision;
    };
            
}
