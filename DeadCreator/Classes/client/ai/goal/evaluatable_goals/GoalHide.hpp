//
//  GoalHide.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#pragma once

#include "GoalEvaluatable.hpp"
#include "Types.hpp"
#include "Geometry.hpp"

namespace realtrick
{
    namespace client
    {
        
        class GoalHide : public GoalEvaluatable
        {
            
        public:
            
			static void findHidePositions(
				const std::vector<cocos2d::Vec2>& vertices,
				cocos2d::Vec2 entityPos,
				cocos2d::Vec2 enemy,
				std::list<cocos2d::Vec2>& hidePositions);

			static bool findHidePosition(
				const std::vector<realtrick::Polygon>& poly,
				HumanBase* owner,
				cocos2d::Vec2 targetPos,
				cocos2d::Vec2& hidePos);

            explicit GoalHide(HumanBase* owner, float character_bias = 0.0f);
			virtual ~GoalHide()
			{}
            
			virtual int evaluate(HumanBase* const owner) override;
            virtual void activate() override;
            virtual GoalStatus process() override;
            virtual void terminate() override;

        };
        
    }
}









