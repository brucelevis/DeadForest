//
//  GoalBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 10..
//
//

#pragma once

#include "cocos2d.h"

namespace realtrick
{
    namespace client
    {
        
        class HumanBase;
        
        class GoalBase
        {
            
        public:
            
            enum class GoalStatus : int { ACTIVE, INACTIVE, COMPLETED, FAILED };
            
        public:
            
            explicit GoalBase(HumanBase* owner) : _owner(owner) {}
            virtual ~GoalBase() = default;
            
            GoalStatus getGoalStatus() const { return _goalStatus; }
            void setGoalStatus(GoalStatus status) { _goalStatus = status; }
            
            bool isCompleted() const { return (_goalStatus == GoalStatus::COMPLETED); }
            bool isActive() const { return (_goalStatus == GoalStatus::ACTIVE); }
            bool isInactive() const { return (_goalStatus == GoalStatus::INACTIVE); }
            bool isFailed() const { return (_goalStatus == GoalStatus::FAILED); }
            
            virtual void activate() = 0;
            virtual GoalStatus process() = 0;
            virtual void terminate() = 0;
            
            virtual GoalStatus processSubgoals()
            {
                while (!_subGoals.empty() &&
                       (_subGoals.back()->isCompleted() || _subGoals.back()->isFailed()))
                {
                    auto recentGoal = _subGoals.back();
                    recentGoal->terminate();
                    CC_SAFE_DELETE(recentGoal);
                    _subGoals.pop_back();
                }
                
                if ( !_subGoals.empty() )
                {
                    auto goalStatus = _subGoals.back()->process();
                    if ( goalStatus == GoalStatus::COMPLETED && _subGoals.size() > 1 )
                    {
                        return GoalStatus::ACTIVE;
                    }
                    return goalStatus;
                }
                
                // if sub goals is empty
                return GoalStatus::COMPLETED;
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
            
        protected:
            
            HumanBase* _owner;
            std::vector<GoalBase*> _subGoals;
            GoalStatus _goalStatus;
            
        };
        
    }
}









