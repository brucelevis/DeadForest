//
//  BrainBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 10..
//
//

#pragma once
#include "Regulator.h"
#include "GoalThink.hpp"
#include <memory>

namespace realtrick
{
    namespace client
    {
        class HumanBase;
		class GoalThink;
        
        class BrainBase
        {
            
        public:
            
			inline GoalThink* getGoalThink() const { return _thinker; }

            explicit BrainBase(HumanBase* owner) 
				: 
				_owner(owner),
				_thinker(new GoalThink(owner)),
				_regulator(0.1f) {}

			virtual ~BrainBase()
			{
				delete _thinker;
				_thinker = nullptr;
			}
            
            virtual void think() = 0;
            
        protected:
            
            HumanBase* _owner;
			GoalThink* _thinker;
			Regulator _regulator;
            
        };
        
    }
}









