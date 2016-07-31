//
//  TriggerSystem2.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    namespace client
    {
        
        class GameTrigger;
        class EntityManager;
        
        class TriggerSystem2
        {
            
            EntityManager* _entityManager;
            std::map<std::string, cocos2d::Rect> _locations;
            std::vector<GameTrigger*> _triggers;
            
        };
        
    }
}

