//
//  GoalWander.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 10..
//
//

#include "GoalBase.hpp"

#include <chrono>


namespace realtrick
{
    namespace client
    {
        
        class GoalWander : public GoalBase
        {
            
        public:
            
            explicit GoalWander(HumanBase* owner) : GoalBase(owner)
            {}
            virtual ~GoalWander() = default;
            
            virtual void activate() override
            {
            }
            
            virtual GoalStatus process() override
            {
                return GoalStatus::ACTIVE;
            }
            
            virtual void terminate() override
            {
            }
            
        private:
            
            std::chrono::high_resolution_clock _start;
            
        };
        
    }
}









