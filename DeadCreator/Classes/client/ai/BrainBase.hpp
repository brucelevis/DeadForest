//
//  BrainBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 10..
//
//

#pragma once

#include <memory>

#include "GoalThink.hpp"


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
            
            explicit BrainBase(HumanBase* owner) :
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
            
            virtual void executeGoal(GoalBase* goal)
            {
                _thinker->terminate();
                _thinker->executeGoal(goal);
            }
            
        protected:
            
            HumanBase* _owner;
            GoalThink* _thinker;
            
        };
        
    }
}









