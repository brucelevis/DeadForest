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
				id(cocos2d::Node::INVALID_TAG)
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
			virtual ~GoalFindWeapon()
			{}
            
			void makeFindItemWeight();
			std::pair<EntityType, FindItemData> getBestItem(float& weight) const;

			virtual int evaluate(HumanBase* const owner) override;
            virtual void activate() override;
            virtual GoalStatus process() override;
            virtual void terminate() override;
            
        private:
			std::vector<float> _weightFindItem;
			std::vector<FindItemData> _findItemMap;

			FindItemData _findWeaponData;
        };
    }
}









