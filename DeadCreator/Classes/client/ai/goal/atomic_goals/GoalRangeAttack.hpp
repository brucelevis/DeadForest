#pragma once

#include "GoalBase.hpp"

namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class GoalRangeAttack : public GoalBase
		{

		public:
			static cocos2d::Vec2 smartMoving(HumanBase* owner);
			static cocos2d::Vec2 dealCrowdMoving(HumanBase* owner);
			static cocos2d::Vec2 makeFormationMoving(HumanBase* owner);

			GoalRangeAttack(
				HumanBase* owner,
				const cocos2d::Vec2& target,
				std::function<cocos2d::Vec2(HumanBase*)> _movingStrategy);
			virtual ~GoalRangeAttack() override
			{}

			virtual void activate() override;
			virtual GoalStatus process() override;
			virtual void terminate() override;

		public:

			cocos2d::Vec2 _target;
			std::chrono::duration<double> _startTime;
			bool _attacked;
			std::function<cocos2d::Vec2(HumanBase*)> _movingStrategy;
		};
	}
}









