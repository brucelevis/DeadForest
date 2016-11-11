#pragma once

#include "GoalBase.hpp"

namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class GoalWalkToDirection : public GoalBase
		{

		public:

			GoalWalkToDirection(
				HumanBase* owner,
				const cocos2d::Vec2& direction,
				float walkTime);

			virtual ~GoalWalkToDirection() override
			{}

			virtual void activate() override;
			virtual GoalStatus process() override;
			virtual void terminate() override;

		private:
			std::chrono::duration<double> _startTime;
			cocos2d::Vec2 _direction;
			float _walkTime;
            
		};
	}
}









