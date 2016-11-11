//
//  GoalFollowLeader.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//


#pragma once


#include <chrono>
#include "GoalEvaluatable.hpp"


namespace realtrick
{
    namespace client
    {
        
        class GoalFollowLeader : public GoalEvaluatable
        {
            
        public:
            
			explicit GoalFollowLeader(HumanBase* const owner, float character_bias = 0.0f);

			virtual ~GoalFollowLeader() override
			{}

			virtual int evaluate(HumanBase* const owner) override;
            virtual void activate() override;
            virtual GoalStatus process() override;
			virtual void terminate() override;

		private:
            
			cocos2d::Vec2 _destination;
			bool _arriveSafeZone;
            
        };
    }
}









