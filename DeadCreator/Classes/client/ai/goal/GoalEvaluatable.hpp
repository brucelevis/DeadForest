//
//  GoalCompositeBase.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 8. 10..
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
            
			inline float getCharacterBias() const { return _characterBias; };

            explicit GoalEvaluatable(HumanBase* owner, float _characterBias)
				:
				GoalCompositeBase(owner),
				_characterBias(_characterBias)
            {
                setGoalType(GoalType::INVALID);
            }

            virtual ~GoalEvaluatable()
            {}
            
			virtual int evaluate(HumanBase* const owner) = 0;

        protected:
            
			float _characterBias;
            
        };
        
    }
}









