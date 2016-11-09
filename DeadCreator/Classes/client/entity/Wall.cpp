#include "Wall.hpp"
#include "Physics.hpp"
#include "Types.hpp"
#include "PhysicsManager.hpp"
using namespace cocos2d;
using namespace realtrick;
using namespace realtrick::client;


Wall::Wall()
{}


Wall::~Wall()
{}


Wall * Wall::create(PhysicsManager* mgr, const std::vector<cocos2d::Vec2>& vertices)
{
	auto ret = new (std::nothrow) Wall();
	if ( ret && ret->init(mgr, vertices) )
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool Wall::init(PhysicsManager* mgr, const std::vector<cocos2d::Vec2>& vertices)
{
	// add box2d polygons
	b2BodyDef groundDef;
	groundDef.type = b2BodyType::b2_staticBody;

    auto size = vertices.size();
	b2Vec2* v = new b2Vec2[size + 1];
	for (int i = 0; i < size - 1; ++i)
	{
		v[i].Set(vertices[i].x, vertices[i].y);
	}
    v[size - 1].Set(vertices.front().x, vertices.front().y);

	b2ChainShape chain;
	chain.CreateChain(v, static_cast<int32>(vertices.size()));

	delete[] v;

	b2FixtureDef groundFixture;
	groundFixture.shape = &chain;
	groundFixture.restitution = 1.0f;
	groundFixture.friction = 0.0f;

    if (!PhysicsBase::initWithPhysicsBody(mgr, groundDef, groundFixture, PhysicsBase::Type::kWall))
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









