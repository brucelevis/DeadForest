//
//  GoalFollowPath.hpp
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
        
        class GoalFollowPath : public GoalCompositeBase
        {
            
        public:
            
            explicit GoalFollowPath(HumanBase* owner);
            virtual ~GoalFollowPath();
            
            virtual void activate() override;
            virtual GoalStatus process() override;
            virtual void terminate() override;
            
        private:
            
            
            
        };
        
    }
}









