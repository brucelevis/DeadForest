#pragma once

#include "GoalEvaluatable.hpp"
#include "Types.hpp"
#include <array>

namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class GoalEquipWeapon : public GoalEvaluatable
		{
            
		public:
            
			GoalEquipWeapon(HumanBase* owner, float character_bias = 0.0f);
			virtual ~GoalEquipWeapon() override
			{}

			void makeEquipItemWeight();
			EntityType getBestItem(float& weight) const;

			virtual int evaluate(HumanBase* const owner) override;
			virtual void activate() override;
			virtual GoalStatus process() override;
			virtual void terminate() override;

		private:
            
			std::chrono::duration<double> _startTime;
			std::array<float, EntityType::MAX> _weightEquipItem;
			EntityType _weaponType;
            
		};
	}
}









