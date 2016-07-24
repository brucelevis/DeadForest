
#include "EntityHuman.hpp"
#include "HumanOwnedAnimations.hpp"
#include "HumanOwnedStates.hpp"
#include "ParamLoader.hpp"
#include "GameManager.hpp"

using namespace realtrick;

EntityHuman::EntityHuman(GameManager* mgr) : DynamicEntity(mgr),
_FSM(nullptr),
_bodyAnimationPlayer(nullptr),
_inputMask(0),
_blood(0),
_maxBlood(0),
_isAlive(false),
_isFovOn(false),
_bodyRot(0.0f),
_walkSpeed(Prm.getValueAsFloat("walkSpeed")),
_runSpeed(Prm.getValueAsFloat("runSpeed")),
_noEventTime(0.0f),
_enduranceTime(0.0f),
_isRun(false),
_footGauge(0.0f),
_isLoad(false)
{
	setEntityType(ENTITY_HUMAN);
	setBoundingRadius(Prm.getValueAsFloat("boundingRadius"));
	setTurnSpeed(Prm.getValueAsFloat("turnSpeed"));
	setMaxSpeed(Prm.getValueAsFloat("maxSpeed"));
}

EntityHuman::~EntityHuman()
{
	/*
	CC_SAFE_DELETE(_bodyAnimationPlayer);
	CC_SAFE_DELETE(_FSM);
	CC_SAFE_RELEASE(_inventory);
	CC_SAFE_RELEASE(_weaponStatus);
	*/
}

bool EntityHuman::init()
{
	_bodyAnimationPlayer = new AnimationPlayer(this, &AnimHumanFistIdleLoop::getInstance(), 4);

	_FSM = new StateMachine<EntityHuman>(this);
	_FSM->setGlobalState(&HumanGlobalState::getInstance());
	_FSM->setCurrState(&HumanFistIdleLoop::getInstance());
	_FSM->changeState(&HumanFistIdleLoop::getInstance());

	_maxBlood = Prm.getValueAsInt("maxBlood");
	_blood = _maxBlood;

	setAlive();

	_enduranceTime = 5.0f;

	return true;
}


EntityHuman* EntityHuman::create(GameManager* mgr)
{
	EntityHuman* ret = new (std::nothrow) EntityHuman(mgr);
	if (ret && ret->init())
	{
		return ret;
	}
	delete(ret);
	ret = nullptr;
	return nullptr;
}


void EntityHuman::update(float dt)
{
	moveEntity(dt);
	rotateEntity(dt);

	if (_FSM) _FSM->update(dt);
	if (_bodyAnimationPlayer) _bodyAnimationPlayer->processAnimation(dt);
}


void EntityHuman::setDead()
{
	_isAlive = false;
}


void EntityHuman::setAlive()
{
	_isAlive = true;
	_blood = _maxBlood;
}


bool EntityHuman::handleMessage(const Telegram& msg)
{
	return _FSM->handleMessage(msg);
}


bool EntityHuman::isIntersectOther(const Vec2& futurePosition, EntityBase* other)
{
	if (other->getEntityType() == ENTITY_HUMAN)
	{
		EntityHuman* human = static_cast<EntityHuman*>(other);
		if (human->isAlive() && physics::intersect(geometry::Circle(futurePosition, getBoundingRadius()), geometry::Circle(human->getWorldPosition(), 20.0f)))
		{
			return true;
		}
	}

	return false;
}


bool EntityHuman::isIntersectWall(const Vec2& futurePosition, const geometry::Polygon& wall)
{
	for (int i = 0; i < wall.vertices.size() - 1; ++i)
	{
		if (physics::intersect(geometry::Circle(futurePosition, getBoundingRadius()), geometry::Segment(wall.vertices[i], wall.vertices[i + 1])))
		{
			return true;
		}
	}
	if (physics::intersect(geometry::Circle(futurePosition, getBoundingRadius()), geometry::Segment(wall.vertices.back(), wall.vertices.front())))
	{
		return true;
	}

	return false;
}


void EntityHuman::rotateEntity(float dt)
{
	if (_heading.dot(_targetHeading) < 0.995f)
	{
		float d = getHeading().cross(_targetHeading);
		if (d > 0)
		{
			Mat3 rotMat;
			rotMat.rotate(MATH_DEG_TO_RAD(_turnSpeed * dt));
			setHeading(rotMat.getTransformedVector(getHeading()));
		}
		else
		{
			Mat3 rotMat;
			rotMat.rotate(-MATH_DEG_TO_RAD(_turnSpeed * dt));
			setHeading(rotMat.getTransformedVector(getHeading()));
		}
	}

	setBodyRot(-physics::getAngleFromZero(getHeading()));
}
