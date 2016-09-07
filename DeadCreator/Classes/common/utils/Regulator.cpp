#include "Regulator.h"
#include "cocos2d.h"


bool Regulator::isReady()
{
	float dt = cocos2d::Director::getInstance()->getDeltaTime();
	
	//cocos2d::log("out Regulator");

	_accumulatedTime += dt;
	if (_accumulatedTime > _duration)
	{
		//cocos2d::log("in Regulator @@@@@@@@@@@@@@");
		_accumulatedTime = 0.0f;
		return true;
	}
	return false;
}
