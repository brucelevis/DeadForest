//
//  GoalFindWeapon.hpp
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
        class GoalFindWeapon : public GoalEvaluatable
        {
        public:
            explicit GoalFindWeapon(HumanBase* owner, float character_bias = 0.0f);
            virtual ~GoalFindWeapon();
            
			void makeFindItemWeight();
			EntityType getBestItem(float& weight) const;

			virtual int evaluate(HumanBase* const owner) override;
            virtual void activate() override;
            virtual GoalStatus process() override;
            virtual void terminate() override;
            
        private:
			std::map<EntityType, float> _weightFindItem;
			EntityType _findWeapon;
			int _findWeaponID;
            
        };
    }
}









