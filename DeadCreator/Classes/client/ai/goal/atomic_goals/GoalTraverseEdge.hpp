//
//  GoalTraverseEdge.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include <chrono>

#include "GoalBase.hpp"

namespace realtrick
{
    namespace client
    {
        
        class GoalTraverseEdge : public GoalBase
        {
            
        public:
            
            explicit GoalTraverseEdge(HumanBase* owner);
            virtual ~GoalTraverseEdge();
            
            virtual void activate() override;
            virtual GoalStatus process() override;
            virtual void terminate() override;
            
        private:
            
            std::chrono::duration<double> _start;
            
        };
        
    }
}









