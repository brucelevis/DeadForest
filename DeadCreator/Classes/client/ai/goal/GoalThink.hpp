//
//  GoalThink.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 10..
//
//

#pragma once

#include "GoalBase.hpp"


namespace realtrick
{
    namespace client
    {
        
        class GoalThink : public GoalBase
        {
            
        public:
            
            GoalThink(HumanBase* owner) : GoalBase(owner)
            {
                /*
                 
                 create sub goals
                 
                 */
            }
            
            virtual ~GoalThink() = default;
            
            virtual void activate() override
            {
                setGoalStatus(GoalStatus::ACTIVE);
            }
            
            virtual GoalStatus process() override
            {
                if ( isInactive() ) activate();
                
                auto subGoalStatus = processSubgoals();
                if ( subGoalStatus == GoalStatus::COMPLETED || subGoalStatus == GoalStatus::FAILED )
                {
                    setGoalStatus(GoalStatus::INACTIVE);
                }
                
                return getGoalStatus();
            }
            
            virtual void terminate() override
            {
                
            }
            
        private:
            
            
        };
        
    }
}









