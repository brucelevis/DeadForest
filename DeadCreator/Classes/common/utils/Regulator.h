#ifndef REGULATOR
#define REGULATOR
//------------------------------------------------------------------------
//
//  Name:   Regulator.h
//
//  Desc:   
//
//  Author: 
//
//------------------------------------------------------------------------

#include "cocos2d.h"


class Regulator
{

public:

	explicit Regulator(float duration) :
		_duration(duration),
		_accumulatedTime(cocos2d::random(0.0f, duration))
	{}

	bool isReady();

private:

	float _duration;
	float _accumulatedTime;

};



#endif