//
//  GoalTraverseEdge.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 17..
//
//

#include <chrono>
#include <memory>
#include "GoalCompositeBase.hpp"
#include "PathEdge.hpp"

namespace realtrick
{
    namespace client
    {
        class GoalMoveToPosition : public GoalCompositeBase
        {
            
        public:
			// precise version
			GoalMoveToPosition(
				HumanBase* const owner,
				cocos2d::Vec2 pos,
				std::shared_ptr<ArrivingData> arrivingData = nullptr);

			virtual ~GoalMoveToPosition() override
			{}

            virtual void activate() override;
            virtual GoalStatus process() override;
			virtual void terminate() override;

        private:
        
			// the position the bot wants to reach
			cocos2d::Vec2 _destination;

			// additional data for precise arriving
			std::shared_ptr<ArrivingData> _arrivingData;
        };
    }
}


