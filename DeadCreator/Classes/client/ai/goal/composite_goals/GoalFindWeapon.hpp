//
//  GoalFindWeapon.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#pragma once

#include "GoalCompositeBase.hpp"
#include "Types.hpp"

namespace realtrick
{
    namespace client
    {
        
        class GoalFindWeapon : public GoalCompositeBase
        {
            
        public:
            
            explicit GoalFindWeapon(HumanBase* owner);
            virtual ~GoalFindWeapon();
            
			int evaluate(HumanBase* const owner);
			void makeFindItemWeight();
			EntityType getBestItem(float& weight) const;

            virtual void activate() override;
            virtual GoalStatus process() override;
            virtual void terminate() override;
            
        private:
            
			std::map<EntityType, float> _weightFindItem;
			EntityType _findWeapon;
            
        };
        
    }
}









