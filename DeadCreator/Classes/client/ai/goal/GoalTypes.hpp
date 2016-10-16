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
			EQUIP_WEAPON = 3,
			FIND_WEAPON = 4,
			RELOAD = 5,
			FOLLOW_PATH = 6,
			TRAVERSE_EDGE = 7,
			SEEK_TO_POS = 8,
			MOVE_TO_POS = 9,
			ATTACK_TARGET = 10,
			MAIN_ATTACK = 11,
			RANGE_ATTACK = 12,
			AVOID = 13,
			ATTACK_TO_DESTINATION = 14,
			RUN_AWAY = 15,
			FOLLOW_PLAYER = 16,
			AVOID_COLLISION = 17,
			MAKE_FORMATION = 18,
			WALK_WITH_AIM = 19
        };
        
    }
}