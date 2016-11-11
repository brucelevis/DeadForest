#pragma once

#include "GoalBase.hpp"

namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class GoalMainAttack : public GoalBase
		{

		public:
			static cocos2d::Vec2 smartMoving(HumanBase* owner);
			static cocos2d::Vec2 roughMoving(HumanBase* owner);
			static cocos2d::Vec2 chaseMoving(HumanBase* owner);
			static cocos2d::Vec2 simpleMoving(HumanBase* owner);

			GoalMainAttack(
				HumanBase* owner,
				const cocos2d::Vec2& target,
				std::function<cocos2d::Vec2(HumanBase*)> movingStrategy);

			virtual ~GoalMainAttack() override
			{}

			virtual void activate() override;
			virtual GoalStatus process() override;
			virtual void terminate() override;

		private:

			cocos2d::Vec2 _target;
			std::chrono::duration<double>	_startTime;
			std::function<cocos2d::Vec2(HumanBase*)> _movingStrategy;
            
		};
	}
}









