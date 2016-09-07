#ifndef ABST_TARGETING_SYSTEM_H
#define ABST_TARGETING_SYSTEM_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   AbstTargetingSystem.h
//
//  Author: insub im (insooneelife@naver.com)
//
//  Desc:   class to select a target from the opponents currently in a bot's
//          perceptive memory.
//-----------------------------------------------------------------------------

#include "cocos2d.h"
#include <list>


namespace realtrick
{
	namespace client
	{
		class HumanBase;

		class AbstTargetingSystem
		{
		protected:

			//the owner of this system
			HumanBase* const	_owner;

			//the current target (this will be null if there is no target assigned)
			HumanBase*			_current_target;

		public:

			explicit AbstTargetingSystem(HumanBase* const owner);

			virtual ~AbstTargetingSystem()
			{}

			//each time this method is called the opponents in the owner's sensory 
			//memory are examined and the closest  is assigned to _current_target.
			//if there are no opponents that have had their memory records updated
			//within the memory span of the owner then the current target is set
			//to null
			virtual void			update();

			//returns true if there is a currently assigned target
			bool					isTargetPresent()const;

			//returns true if the target is within the field of view of the owner
			virtual bool			isTargetWithinFOV()const;

			//returns true if there is unobstructed line of sight between the target
			//and the owner
			virtual bool			isTargetAttackable()const;

			//returns the position the target was last seen. Throws an exception if
			//there is no target currently assigned
			virtual cocos2d::Vec2	getLastRecordedPosition()const;

			//returns the amount of time the target has been in the field of view
			virtual std::chrono::duration<double>			getTimeTargetHasBeenVisible()const;

			//returns the amount of time the target has been out of view
			virtual std::chrono::duration<double>			getTimeTargetHasBeenOutOfView()const;

			//returns a pointer to the target. null if no target current.
			HumanBase*				getTarget()const;

			void					setTarget(HumanBase* target);

			//sets the target pointer to null
			void					clearTarget();
		};
	}
}
#endif
