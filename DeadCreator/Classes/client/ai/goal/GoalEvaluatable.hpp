//
//  GoalCompositeBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 10..
//
//

#pragma once

#include "cocos2d.h"
#include "GoalCompositeBase.hpp"

namespace realtrick
{
    namespace client
    {
        
        class GoalEvaluatable : public GoalCompositeBase
        {
            
        public:
            
			inline float getCharacterBias() const { return _character_bias; };

            explicit GoalEvaluatable(HumanBase* owner, float character_bias) 
				:
				GoalCompositeBase(owner),
				_character_bias(character_bias)
            {
                setGoalType(GoalType::INVALID);
            }

            virtual ~GoalEvaluatable()
            {}
            
			virtual int evaluate(HumanBase* const owner) = 0;

        protected:
            
			float _character_bias;
            
        };
        
    }
}









