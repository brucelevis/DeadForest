//
//  GoalCompositeBase.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 8. 10..
//
//

#pragma once

#include "cocos2d.h"
#include "GoalBase.hpp"

namespace realtrick
{
    namespace client
    {
        
        class GoalCompositeBase : public GoalBase
        {
            
        public:
            
            explicit GoalCompositeBase(HumanBase* owner) : GoalBase(owner)
            {
                setGoalType(GoalType::INVALID);
            }
            virtual ~GoalCompositeBase()
            {
                _subGoals.clear();
            }
            
            void addSubgoal(GoalBase* goal) { _subGoals.push_back(goal); }
            
            void removeAllSubgoals()
            {
                for ( auto& goal : _subGoals )
                    goal->terminate();
                _subGoals.clear();
            }
            
            virtual void activate() = 0;
            virtual GoalStatus process() = 0;
            virtual void terminate() = 0;
            
        protected:
            
            GoalStatus processSubgoals()
            {
                if ( _subGoals.empty() ) return GoalStatus::COMPLETED;
                
                auto goalStatus = _subGoals.back()->process();
                if ( goalStatus == GoalStatus::COMPLETED || goalStatus == GoalStatus::FAILED )
                {
                    auto recentGoal = _subGoals.back();
                    recentGoal->terminate();
                    recentGoal->setGoalStatus(GoalStatus::INACTIVE);
                    _subGoals.pop_back();
                    
                    if ( _subGoals.empty() ) return goalStatus;
                    else return GoalStatus::ACTIVE;
                }
                return goalStatus;
            }
            
        protected:
            
            std::vector<GoalBase*> _subGoals;
            
        };
        
    }
}









