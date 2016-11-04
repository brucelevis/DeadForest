#include "PhysicsManager.hpp"
#include "Game.hpp"
#include "EntityBase.hpp"
#include "ItemBase.hpp"
#include "HumanBase.hpp"
#include "PhysicsBase.hpp"
#include "Wall.hpp"

using namespace realtrick;
using namespace realtrick::client;
using namespace cocos2d;


bool QueryWallByAABB::ReportFixture(b2Fixture* fixture)
{
	auto userData = fixture->GetBody()->GetUserData();
	if (userData)
	{
		PhysicsBase* physic = static_cast<PhysicsBase*>(userData);
		if (physic->getType() == PhysicsBase::kWall)
		{
			walls.push_back(static_cast<Wall*>(physic));
		}
	}
	return true;
}

bool QueryEntityByAABB::ReportFixture(b2Fixture* fixture)
{
	auto userData = fixture->GetBody()->GetUserData();
	if (userData)
	{
		PhysicsBase* physic = static_cast<PhysicsBase*>(userData);
		if (physic->getType() == PhysicsBase::kHuman)
		{
			entities.push_back(static_cast<HumanBase*>(physic));
		}
		else if (physic->getType() == PhysicsBase::kItem)
		{
			entities.push_back(static_cast<ItemBase*>(physic));
		}
	}
	return true;
}

float32 QueryWallByRayCast::ReportFixture(
	b2Fixture* fixture,
	const b2Vec2& point,
	const b2Vec2& normal,
	float32 fraction)
{
	auto userData = fixture->GetBody()->GetUserData();
	if (userData)
	{
		PhysicsBase* physic = static_cast<PhysicsBase*>(userData);
		if (physic->getType() == PhysicsBase::kWall)
		{
			walls.push_back(static_cast<Wall*>(physic));
		}
	}
	return true;
}


float32 QueryEntityByRayCast::ReportFixture(
	b2Fixture* fixture,
	const b2Vec2& point,
	const b2Vec2& normal,
	float32 fraction)
{
	auto userData = fixture->GetBody()->GetUserData();
	if (userData)
	{
		PhysicsBase* physic = static_cast<PhysicsBase*>(userData);
		if (physic->getType() == PhysicsBase::kHuman)
		{
			entities.push_back(static_cast<HumanBase*>(physic));
		}
		else if (physic->getType() == PhysicsBase::kItem)
		{
			entities.push_back(static_cast<ItemBase*>(physic));
		}
	}
	return true;
}


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
	//check if both fixtures were balls
	void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	if (bodyUserDataA && bodyUserDataB)
	{
		PhysicsBase* a = static_cast<PhysicsBase*>(bodyUserDataA);
		PhysicsBase* b = static_cast<PhysicsBase*>(bodyUserDataB);

		if (a->getType() == PhysicsBase::kHuman && b->getType() == PhysicsBase::kHuman)
		{
			auto humanA = static_cast<HumanBase*>(a);
			auto humanB = static_cast<HumanBase*>(b);
		}

		else if (a->getType() == PhysicsBase::kHuman && b->getType() == PhysicsBase::kItem)
		{
			auto human = static_cast<HumanBase*>(a);
			auto item = static_cast<ItemBase*>(b);

			if (human->getEntityType() == EntityType::ENTITY_PLAYER)
			{
				ItemAndOwner data;
				data.owner = human;
				data.item = item;
				human->getGame()->pushLogic(0.0, MessageType::PUSH_ITEM_TO_INVENTORY, &data);
			}
		}

		else if (a->getType() == PhysicsBase::kHuman && b->getType() == PhysicsBase::kWall)
		{
			auto human = static_cast<HumanBase*>(a);
			auto wall = static_cast<Wall*>(b);
		}

		else if (a->getType() == PhysicsBase::kWall && b->getType() == PhysicsBase::kHuman)
		{
			auto wall = static_cast<Wall*>(a);
			auto human = static_cast<HumanBase*>(b);
		}

		else if (a->getType() == PhysicsBase::kItem && b->getType() == PhysicsBase::kHuman)
		{
			auto item = static_cast<ItemBase*>(a);
			auto human = static_cast<HumanBase*>(b);

			if (human->getEntityType() == EntityType::ENTITY_PLAYER)
			{
				ItemAndOwner data;
				data.owner = human;
				data.item = item;
				human->getGame()->pushLogic(0.0, MessageType::PUSH_ITEM_TO_INVENTORY, &data);
			}
		}
	}
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


void PhysicsManager::ReserveRemoveBody(b2Body* body)
{
    body->SetUserData(nullptr);
    _removedBodies.push_back(body);
}


void PhysicsManager::RemoveReservedBodies()
{
    for ( auto& body : _removedBodies )
        _world->DestroyBody(body);
    _removedBodies.clear();
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

std::vector<Wall*> PhysicsManager::queryWallsAABB(
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

std::vector<Wall*> PhysicsManager::queryWallsRayCast(
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
