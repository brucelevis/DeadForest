//
//  GoalFindWeapon.hpp
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
        
        class GoalFindWeapon : public GoalCompositeBase
        {
            
        public:
            
            explicit GoalFindWeapon(HumanBase* owner);
            virtual ~GoalFindWeapon();
            
            virtual void activate() override;
            virtual GoalStatus process() override;
            virtual void terminate() override;
            
        private:
            
            
            
        };
        
    }
}









