//
//  GoalHuntTarget.hpp
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
     
        class GoalReadyToFight;
        
        class GoalHuntTarget : public GoalCompositeBase
        {
            
        public:
            
            explicit GoalHuntTarget(HumanBase* owner);
            virtual ~GoalHuntTarget();
            
            virtual void activate() override;
            virtual GoalStatus process() override;
            virtual void terminate() override;
            
        private:
            
            GoalReadyToFight* _goalReadyToFight;
            
        };
        
    }
}









