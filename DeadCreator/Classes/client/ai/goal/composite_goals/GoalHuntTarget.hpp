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
        
        class GoalHuntTarget : public GoalCompositeBase
        {
            
        public:
            
            explicit GoalHuntTarget(HumanBase* owner);
			virtual ~GoalHuntTarget()
			{}
            
            virtual void activate() override;
            virtual GoalStatus process() override;
            virtual void terminate() override;
            
        private:
            
			//this value is set to true if the last visible position of the target
			//bot has been searched without success
			bool  _last_visit_visible_pos_tried;
            
        };
        
    }
}









