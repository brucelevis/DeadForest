//
//  GoalThink.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 10..
//
//

#pragma once

#include "GoalCompositeBase.hpp"

namespace realtrick
{
    namespace client
    {
        
        class GoalThink : public GoalCompositeBase
        {
            
        public:
            
            explicit GoalThink(HumanBase* owner);
            virtual ~GoalThink();
            
            virtual void activate() override;
            virtual GoalStatus process() override;
            virtual void terminate() override;
            
        private:
            
            
        };
        
    }
}









