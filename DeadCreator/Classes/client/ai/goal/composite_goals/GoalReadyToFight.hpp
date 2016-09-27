//
//  GoalReadyToFight.hpp
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
        
        class GoalFindWeapon;
        
        class GoalReadyToFight : public GoalCompositeBase
        {
            
        public:
			static int evaluate(HumanBase* const owner);

            explicit GoalReadyToFight(HumanBase* owner);
            virtual ~GoalReadyToFight();
            
            virtual void activate() override;
            virtual GoalStatus process() override;
            virtual void terminate() override;
            
			void makeEquipItemWeight();
			void makeFindItemWeight();

			EntityType getBestItem(const std::map<EntityType, float>& weightMap) const;

		private:

			std::map<EntityType, float> _weightEquipItem;
			std::map<EntityType, float> _weightFindItem;
        };
        
    }
}









