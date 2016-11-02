#include "PhysicsManager.h"
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <iostream>
#include "Game.hpp"
#include "EntityBase.hpp"

using namespace realtrick;
using namespace realtrick::client;
using namespace cocos2d;

bool PhysicsManager::CheckContact(
	const b2Shape* shapeA, int32 indexA,
	const b2Shape* shapeB, int32 indexB,
	const b2Transform& xfA, const b2Transform& xfB,
	b2DistanceOutput& output)
{
	b2DistanceInput input;
	input.proxyA.Set(shapeA, indexA);
	input.proxyB.Set(shapeB, indexB);
	input.transformA = xfA;
	input.transformB = xfB;
	input.useRadii = true;

	b2SimplexCache cache;
	cache.count = 0;

	b2Distance(&output, &cache, &input);

	return output.distance < 10.0f * b2_epsilon;
}


PhysicsManager::PhysicsManager(float worldX, float worldY)
	:
	_pointCount(0)
{
	b2Vec2 gravity;
	gravity.Set(0.0f, 0.0f);
	_world = new b2World(gravity);
	_destruction_listener.physics = this;
	_world->SetDestructionListener(&_destruction_listener);
	_world->SetContactListener(this);
	_world->SetDebugDraw(nullptr);

	// Ground body
	{
		b2BodyDef bd;
		//bd.position.Set(worldX / 2, worldY / 2);
		_ground = _world->CreateBody(&bd);

		b2Vec2 points[] =
		{
			b2Vec2(0, 0),
			b2Vec2(worldX, 0),
			b2Vec2(worldX, worldY),
			b2Vec2(0, worldY)
		};

		for (int i = 0; i < 4; i++)
		{
			int j = (i + 1) % 4;
			b2EdgeShape shape;
			shape.Set(points[i], points[j]);
			_ground->CreateFixture(&shape, 0.0f);
		}
	}
}

void PhysicsManager::Step()
{
	_world->Step(
		_settings.hz,
		_settings.velocityIterations,
		_settings.positionIterations);
}



void PhysicsManager::BeginContact(b2Contact* contact)
{
	// 충돌처리 ~
	/*
	//check if both fixtures were balls
	void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	if (bodyUserDataA && bodyUserDataB)
	{
		Entity* entA = static_cast<Entity*>(bodyUserDataA);
		Entity* entB = static_cast<Entity*>(bodyUserDataB);

		if (entA->getType() == Entity::Type::kHunter && entB->getType() == Entity::Type::kHunter)
			World::collide(*static_cast<Hunter*>(entA), *static_cast<Hunter*>(entB));

		else if (entA->getType() == Entity::Type::kHunter && entB->getType() == Entity::Type::kProjectile)
			World::collide(*static_cast<Hunter*>(entA), *static_cast<Projectile*>(entB));

		else if (entA->getType() == Entity::Type::kProjectile && entB->getType() == Entity::Type::kHunter)
			World::collide(*static_cast<Hunter*>(entB), *static_cast<Projectile*>(entA));

		else if (entA->getType() == Entity::Type::kProjectile && entB->getType() == Entity::Type::kPrey)
			World::collide(*static_cast<Projectile*>(entA), *static_cast<Prey*>(entB));

		else if (entA->getType() == Entity::Type::kPrey && entB->getType() == Entity::Type::kProjectile)
			World::collide(*static_cast<Projectile*>(entB), *static_cast<Prey*>(entA));

		else if (entA->getType() == Entity::Type::kHunter && entB->getType() == Entity::Type::kWall)
			World::collide(*static_cast<Hunter*>(entA), *static_cast<Wall*>(entB));

		else if (entA->getType() == Entity::Type::kWall && entB->getType() == Entity::Type::kHunter)
			World::collide(*static_cast<Hunter*>(entB), *static_cast<Wall*>(entA));
	}
	*/
}

void PhysicsManager::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	const b2Manifold* manifold = contact->GetManifold();

	if (manifold->pointCount == 0)
	{
		return;
	}

	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	b2PointState state1[b2_maxManifoldPoints], state2[b2_maxManifoldPoints];
	b2GetPointStates(state1, state2, oldManifold, manifold);

	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);

	for (int32 i = 0; i < manifold->pointCount && _pointCount < 2048; ++i)
	{
		ContactPoint* cp = _points + _pointCount;
		cp->fixtureA = fixtureA;
		cp->fixtureB = fixtureB;
		cp->position = worldManifold.points[i];
		cp->normal = worldManifold.normal;
		cp->state = state2[i];
		cp->normalImpulse = manifold->points[i].normalImpulse;
		cp->tangentImpulse = manifold->points[i].tangentImpulse;
		cp->separation = worldManifold.separations[i];
		++_pointCount;
	}
}


b2Body* PhysicsManager::CreateApplyForceBody(float x, float y, b2Shape* shape)
{
	// Create body
	b2BodyDef bd;
	bd.position.Set(x, y);
	bd.type = b2BodyType::b2_dynamicBody;
	bd.allowSleep = false;

	b2Body* body = _world->CreateBody(&bd);

	b2FixtureDef fd;
	fd.shape = shape;
	fd.density = 1.0f;
	fd.friction = 0.0f;
	fd.restitution = 0.0f;
	body->CreateFixture(&fd);

	// Apply force
	float32 gravity = 10.0f;
	float32 I = body->GetInertia();
	float32 mass = body->GetMass();

	// For a circle: I = 0.5 * m * r * r ==> r = sqrt(2 * I / m)
	float32 radius = b2Sqrt(2.0f * I / mass);

	b2FrictionJointDef jd;
	jd.localAnchorA.SetZero();
	jd.localAnchorB.SetZero();
	jd.bodyA = _ground;
	jd.bodyB = body;
	jd.collideConnected = true;
	jd.maxForce = mass * gravity;
	jd.maxTorque = mass * radius * gravity;

	_world->CreateJoint(&jd);

	return body;
}

b2Body* PhysicsManager::CreateBody(float x, float y, b2BodyType type, b2Shape* shape, bool sensor)
{
	b2BodyDef bd;
	bd.position.Set(x, y);
	bd.type = type;
	bd.allowSleep = false;

	b2Body* body = _world->CreateBody(&bd);

	b2FixtureDef fd;
	fd.shape = shape;
	fd.density = 20.0f;
	fd.friction = 1.0f;
	fd.restitution = 0.0f;
	fd.isSensor = sensor;
	body->CreateFixture(&fd);
	return body;
}

void PhysicsManager::RemoveBody(b2Body* body)
{
	body->GetWorld()->DestroyBody(body);
}

std::vector<EntityBase*> PhysicsManager::queryEntitiesAABB(
	const cocos2d::Vec2& position,
	float halfWidth,
	float halfHeight) const
{
	QueryEntityByAABB query;
	b2AABB aabb;
	aabb.lowerBound = b2Vec2(position.x, position.y) - b2Vec2(halfWidth, halfHeight);
	aabb.upperBound = b2Vec2(position.x, position.y) + b2Vec2(halfWidth, halfHeight);
	_world->QueryAABB(&query, aabb);

	return query.entities;
}

std::vector<b2Body*> PhysicsManager::queryWallsAABB(
	const cocos2d::Vec2& pos,
	float halfWidth,
	float halfHeight) const
{
	QueryWallByAABB query;
	b2AABB aabb;
	aabb.lowerBound = b2Vec2(pos.x, pos.y) - b2Vec2(halfWidth, halfHeight);
	aabb.upperBound = b2Vec2(pos.x, pos.y) + b2Vec2(halfWidth, halfHeight);
	_world->QueryAABB(&query, aabb);

	return query.walls;
}

std::vector<EntityBase*> PhysicsManager::queryEntitiesRayCast(
	const cocos2d::Vec2& start,
	const cocos2d::Vec2& finish) const
{
	QueryEntityByRayCast query;
	_world->RayCast(
		&query,
		b2Vec2(start.x, start.y),
		b2Vec2(finish.x, finish.y));

	return query.entities;
}

std::vector<b2Body*> PhysicsManager::queryWallsRayCast(
	const cocos2d::Vec2& start,
	const cocos2d::Vec2& finish) const
{
	QueryWallByRayCast query;
	_world->RayCast(
		&query,
		b2Vec2(start.x, start.y),
		b2Vec2(finish.x, finish.y));

	return query.walls;
}