//
//  BrainBase.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 8. 10..
//
//

#pragma once
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
				_thinker(new GoalThink(owner))
			{}

			virtual ~BrainBase()
			{
				delete _thinker;
				_thinker = nullptr;
			}
            
			virtual void think()
			{
				
					_thinker->process();
			}
            
        protected:
            
            HumanBase* _owner;
			GoalThink* _thinker;
            
        };
        
    }
}









