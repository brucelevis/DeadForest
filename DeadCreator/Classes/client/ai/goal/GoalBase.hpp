//
//  GoalBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 10..
//
//

#pragma once

#include "cocos2d.h"
#include "GoalTypes.hpp"

namespace realtrick
{
    namespace client
    {
        
        class HumanBase;
        
        enum class GoalStatus : int { ACTIVE, INACTIVE, COMPLETED, FAILED };
        
        class GoalBase
        {
            
        public:
            
            explicit GoalBase(HumanBase* owner) : _owner(owner)
            {
                setGoalStatus(GoalStatus::INACTIVE);
                setGoalType(GoalType::INVALID);
                _isCompositeGoal = false;
            }
            virtual ~GoalBase() = default;
            
			HumanBase* getOwner() const { return _owner; }

            GoalStatus getGoalStatus() const { return _goalStatus; }
            void setGoalStatus(GoalStatus status) { _goalStatus = status; }
            
            GoalType getGoalType() const { return _goalType; }
            void setGoalType(GoalType goalType) { _goalType = goalType; }
            
            void setEvaluator(const std::function<int(HumanBase*)>& evaluator) { _evaluator = evaluator; }
            int getWeight() { return (_evaluator ? _evaluator(_owner) : 0); }
            
            std::string getGoalName() const { return _goalName; }
            
            bool isCompleted() const { return (_goalStatus == GoalStatus::COMPLETED); }
            bool isActive() const { return (_goalStatus == GoalStatus::ACTIVE); }
            bool isInactive() const { return (_goalStatus == GoalStatus::INACTIVE); }
            bool isFailed() const { return (_goalStatus == GoalStatus::FAILED); }
            bool isCompositeGoal() const { return _isCompositeGoal; }
            
            virtual void activate() = 0;
            virtual GoalStatus process() = 0;
            virtual void terminate() = 0;
    
        protected:
            
            void setGoalName(const std::string& name) { _goalName = name; }
            
        protected:
            
            HumanBase* _owner;
            GoalStatus _goalStatus;
            GoalType _goalType;
            std::function<int(HumanBase*)> _evaluator;
            bool _isCompositeGoal;
            std::string _goalName;
            
        };
        
    }
}









