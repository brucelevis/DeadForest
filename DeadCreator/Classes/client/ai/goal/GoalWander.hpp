//
//  GoalWander.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 10..
//
//

#include <chrono>

#include "GoalBase.hpp"

namespace realtrick
{
    namespace client
    {
        
        class GoalWander : public GoalBase
        {
            
        public:
            
            explicit GoalWander(HumanBase* owner);
            virtual ~GoalWander();
            
            virtual void activate() override;
            virtual GoalStatus process() override;
            virtual void terminate() override;
            
        private:
            
            std::chrono::duration<double> _start;
            
        };
        
    }
}









