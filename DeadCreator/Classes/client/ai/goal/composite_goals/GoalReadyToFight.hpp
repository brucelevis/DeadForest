//
//  GoalReadyToFight.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#pragma once

#include "GoalCompositeBase.hpp"

namespace realtrick
{
    namespace client
    {
        
        class GoalFindWeapon;
        
        class GoalReadyToFight : public GoalCompositeBase
        {
            
        public:
            
            explicit GoalReadyToFight(HumanBase* owner);
            virtual ~GoalReadyToFight();
            
            virtual void activate() override;
            virtual GoalStatus process() override;
            virtual void terminate() override;
                   
        };
        
    }
}









