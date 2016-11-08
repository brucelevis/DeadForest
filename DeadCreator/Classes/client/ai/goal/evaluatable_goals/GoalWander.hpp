//
//  GoalWander.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#pragma once

#include "GoalEvaluatable.hpp"
#include "Types.hpp"

namespace realtrick
{
    namespace client
    {
        class GoalWander : public GoalEvaluatable
        {
        public:
            explicit GoalWander(HumanBase* owner, float character_bias = 0.0f);
			virtual ~GoalWander()
			{}
            
			virtual int evaluate(HumanBase* const owner) override;
            virtual void activate() override;
            virtual GoalStatus process() override;
            virtual void terminate() override;
            
        private:

			cocos2d::Vec2 _destination;
			std::chrono::duration<double> _startTime;
        };
    }
}









