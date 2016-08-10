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
            
            explicit GoalBase(HumanBase* owner) : _owner(owner)
            {
                setGoalStatus(GoalStatus::INACTIVE);
            }
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
    
        protected:
            
            HumanBase* _owner;
            GoalStatus _goalStatus;
            
        };
        
    }
}









