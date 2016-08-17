//
//  GoalTypes.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#pragma once

namespace realtrick
{
    namespace client
    {
        
        enum class GoalType
        {
            INVALID = -1,
            THINK = 0,
            HUNT_TARGET = 1,
            WANDER = 2,
            READY_TO_FIGHT = 3,
            FIND_WEAPON = 4,
            FOLLOW_PATH = 5,
            TRAVERSE_EDGE = 6,
        };
        
    }
}