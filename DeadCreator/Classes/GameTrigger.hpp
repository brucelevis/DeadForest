//
//  GameTrigger.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 18..
//
//

#pragma once

#include <vector>
#include "EntityType.hpp"

namespace realtrick
{
    
    class ConditionBase;
    class ActionBase;
    
    class GameTrigger
    {
        
    public:
        
        void drawCondition();
        void drawActions();
        
    private:
        
        std::vector<PlayerType> _players;
        std::vector<ConditionBase*> _conditions;
        std::vector<ActionBase*> _actions;
        
    };
    
}