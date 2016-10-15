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
		struct FindItemData
		{
			FindItemData()
				:
				distance(std::numeric_limits<float>::max()),
				pos(),
				id(-1)
			{}

			FindItemData(float distance, cocos2d::Vec2 pos,	int id)
				:
				distance(distance),
				pos(pos),
				id(id)
			{}

			float distance;
			cocos2d::Vec2 pos;
			int id;
		};

        class GoalFindWeapon : public GoalEvaluatable
        {
        public:
            explicit GoalFindWeapon(HumanBase* owner, float character_bias = 0.0f);
            virtual ~GoalFindWeapon();
            
			void makeFindItemWeight();
			std::pair<EntityType, FindItemData> getBestItem(float& weight) const;

			virtual int evaluate(HumanBase* const owner) override;
            virtual void activate() override;
            virtual GoalStatus process() override;
            virtual void terminate() override;
            
        private:
			std::map<EntityType, float> _weightFindItem;
			std::map<EntityType, FindItemData> _findItemMap;
			EntityType _findWeapon;
			FindItemData _findWeaponData;
            
        };
    }
}









