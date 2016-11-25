#include "Regulator.h"
#include "cocos2d.h"


bool Regulator::isReady()
{
	float dt = cocos2d::Director::getInstance()->getDeltaTime();
	
	_accumulatedTime += dt;
	if (_accumulatedTime > _duration)
	{
		_accumulatedTime = 0.0f;
		return true;
	}
	return false;
}
