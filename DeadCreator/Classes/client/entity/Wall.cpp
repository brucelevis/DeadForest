#include "Wall.hpp"
#include "Physics.hpp"
#include "Types.hpp"

using namespace cocos2d;
using namespace realtrick;
using namespace realtrick::client;


Wall::Wall(b2World* world) : _world(world)
{}


Wall::~Wall()
{}


Wall * Wall::create(b2World* world, const std::vector<cocos2d::Vec2>& vertices)
{
	auto ret = new (std::nothrow) Wall(world);
	if ( ret && ret->init(vertices) )
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool Wall::init(const std::vector<cocos2d::Vec2>& vertices)
{
	// add box2d polygons
	b2BodyDef groundDef;
	groundDef.type = b2BodyType::b2_staticBody;
	auto ground = _world->CreateBody(&groundDef);

	b2Vec2* v = new b2Vec2[vertices.size()];
	for (int i = 0; i < vertices.size(); ++i)
	{
		v[i].Set(vertices[i].x, vertices[i].y);
	}

	b2ChainShape chain;
	chain.CreateChain(v, vertices.size());

	delete[] v;

	b2FixtureDef groundFixture;
	groundFixture.shape = &chain;
	groundFixture.restitution = 0.1f;

	ground->CreateFixture(&groundFixture);

	if (!PhysicsBase::initWithPhysicsBody(_world, groundDef, groundFixture, EntityType::ENTITY_WALL))
		return false;
	return true;
}



Vec2 Wall::getVelocity() const 
{
	return Vec2::ZERO;
}

void Wall::setVelocity(const Vec2& velocity) 
{}

Vec2 Wall::getWorldPosition() const 
{
	auto bodyPos = _body->GetPosition();
	return Vec2(bodyPos.x, bodyPos.y);
}

void Wall::setWorldPosition(const Vec2& pos) 
{
	_body->SetTransform(b2Vec2(pos.x, pos.y), _body->GetAngle());
}

cocos2d::Vec2 Wall::getHeading() const
{
	return cocos2d::Vec2(cos(_body->GetAngle()), sin(_body->GetAngle()));
}

float Wall::getRotationZ() const
{
	return MATH_RAD_TO_DEG(_body->GetAngle());
}

void Wall::setHeading(const cocos2d::Vec2& heading)
{
	_body->SetTransform(_body->GetPosition(), physics::getRadFromZero(heading));
}