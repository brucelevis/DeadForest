#pragma once

#include "GoalEvaluatable.hpp"
#include "Types.hpp"
#include <queue>

namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class GoalProvoke : public GoalEvaluatable
		{

		public:

			GoalProvoke(HumanBase* owner, float character_bias = 0.0f);
			virtual ~GoalProvoke() override
			{}

			virtual int evaluate(HumanBase* const owner) override;
			virtual void activate() override;
			virtual GoalStatus process() override;
			virtual void terminate() override;

		private:
            
			cocos2d::Vec2 _savePos;
			bool _arrivedTargetRange;
			bool _finishedAvoid;
			std::queue<GoalBase*> _queuedGoals;
            
		};
        
	}
}









