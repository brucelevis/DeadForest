#ifndef GOAL_NETWORK_H
#define GOAL_NETWORK_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   GoalNetwork.h
//
//  Author: insub im (insooneelife@naver.com)
//
//  Desc:   Networking goal strategy for multiple ai agents.
//
//-----------------------------------------------------------------------------

#include "cocos2d.h"
#include <list>


namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class GoalNetwork
		{
		public:
			static void staticInitConstants();
			static cocos2d::Vec2 queryFormationPos(HumanBase* const owner, HumanBase* const leader, int ownerIdx);

			static const int kNumOfMaxFormations = 24;
			static const int kNumOfMaxFollowers = 8;
			static const float kFormationDistance;
			static cocos2d::Vec2 kFormations[kNumOfMaxFormations];

			explicit GoalNetwork() 
			{}

			virtual ~GoalNetwork()
			{}

			virtual void process() {}


		

		};
	}
}
#endif
