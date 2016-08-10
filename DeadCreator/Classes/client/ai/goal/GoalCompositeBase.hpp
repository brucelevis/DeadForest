//
//  GoalCompositeBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 10..
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
            
            explicit GoalCompositeBase(HumanBase* owner) : GoalBase(owner) {}
            virtual ~GoalCompositeBase()
            {
                removeAllSubgoals();
            }
            
            void addSubgoal(GoalBase* goal) { _subGoals.push_back(goal); }
            
            void removeAllSubgoals()
            {
                for ( auto& goal : _subGoals )
                {
                    goal->terminate();
                    CC_SAFE_DELETE(goal);
                }
                _subGoals.clear();
            }
            
            virtual void activate() = 0;
            virtual GoalStatus process() = 0;
            virtual void terminate() = 0;
            
        protected:
            
            GoalStatus processSubgoals()
            {
                if ( !_subGoals.empty() )
                {
                    auto goalStatus = _subGoals.back()->process();
                    if ( goalStatus == GoalStatus::COMPLETED || goalStatus == GoalStatus::FAILED )
                    {
                        auto recentGoal = _subGoals.back();
                        recentGoal->terminate();
                        CC_SAFE_DELETE(recentGoal);
                        _subGoals.pop_back();
    
                        if ( _subGoals.size() > 1 )
                        {
                            return GoalStatus::ACTIVE;
                        }
                    }
                    return goalStatus;
                }
                
                // if sub goals is empty
                return GoalStatus::COMPLETED;
            }
            
        protected:
            
            std::vector<GoalBase*> _subGoals;
            
        };
        
    }
}









