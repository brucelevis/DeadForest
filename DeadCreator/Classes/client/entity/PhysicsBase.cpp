#include "PhysicsBase.hpp"
#include "Game.hpp"

using namespace realtrick;
using namespace realtrick::client;

PhysicsBase::PhysicsBase() :
	_body(nullptr)
{}


PhysicsBase::~PhysicsBase()
{}


bool PhysicsBase::initWithPhysicsBody(
	b2World * world, const b2BodyDef & bd, const b2FixtureDef & fd, int type)
{
	_body = world->CreateBody(&bd);
	_body->CreateFixture(&fd);
	_body->SetLinearDamping(10.0f);
	_body->SetAngularDamping(20.0f);
	_body->SetUserData(this);
	_type = type;

	return true;
}
