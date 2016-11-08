#include "PhysicsBase.hpp"
#include "Game.hpp"
#include "PhysicsManager.hpp"
using namespace realtrick;
using namespace realtrick::client;

PhysicsBase::PhysicsBase() :
	_body(nullptr)
{}


PhysicsBase::~PhysicsBase()
{
    if ( _body )
    {
        _physicsManager->ReserveRemoveBody(_body);
    }
}


bool PhysicsBase::initWithPhysicsBody(PhysicsManager* mgr, const b2BodyDef & bd, const b2FixtureDef & fd, int type)
{
    _physicsManager = mgr;
    
	_body = mgr->GetPhysicsWorld()->CreateBody(&bd);
	_body->CreateFixture(&fd);
	_body->SetLinearDamping(10.0f);
	_body->SetAngularDamping(20.0f);
	_body->SetUserData(this);
	_type = type;

	return true;
}
