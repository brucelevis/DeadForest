/*
* Copyright (c) 2006-2010 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include "Box2D/Collision/b2Collision.h"
#include "Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.h"
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Box2D.h>
#include <vector>
#include "EntityBase.hpp"

namespace realtrick
{
	namespace client
	{
		struct Settings
		{
			Settings()
			{
				hz = 0.016f;
				velocityIterations = 8;
				positionIterations = 3;
				drawShapes = true;
				drawJoints = false;
				drawAABBs = true;
				drawContactPoints = true;
				drawContactNormals = true;
				drawContactImpulse = false;
				drawFrictionImpulse = false;
				drawCOMs = true;
				drawStats = false;
				drawProfile = false;
				enableWarmStarting = true;
				enableContinuous = true;
				enableSubStepping = false;
				enableSleep = true;
				pause = false;
				singleStep = false;
			}

			float32 hz;
			int32 velocityIterations;
			int32 positionIterations;
			bool drawShapes;
			bool drawJoints;
			bool drawAABBs;
			bool drawContactPoints;
			bool drawContactNormals;
			bool drawContactImpulse;
			bool drawFrictionImpulse;
			bool drawCOMs;
			bool drawStats;
			bool drawProfile;
			bool enableWarmStarting;
			bool enableContinuous;
			bool enableSubStepping;
			bool enableSleep;
			bool pause;
			bool singleStep;
		};

		class PhysicsManager;
		class DestructionListener : public b2DestructionListener
		{
		public:
			void SayGoodbye(b2Fixture* fixture) { B2_NOT_USED(fixture); }
			void SayGoodbye(b2Joint* joint) { B2_NOT_USED(joint); }

			PhysicsManager* physics;
		};

		struct ContactPoint
		{
			b2Fixture* fixtureA;
			b2Fixture* fixtureB;
			b2Vec2 normal;
			b2Vec2 position;
			b2PointState state;
			float32 normalImpulse;
			float32 tangentImpulse;
			float32 separation;
		};

		class EntityBase;
		struct QueryWallByAABB : public b2QueryCallback
		{
			std::vector<b2Body*> walls;

			virtual bool ReportFixture(b2Fixture* fixture) override
			{
				if (fixture->GetBody()->GetUserData() == nullptr)
					walls.push_back(fixture->GetBody());
				return true;
			}
		};

		struct QueryEntityByAABB : public b2QueryCallback
		{
			std::vector<EntityBase*> entities;

			virtual bool ReportFixture(b2Fixture* fixture) override
			{
				if (fixture->GetBody()->GetUserData())
					entities.push_back(static_cast<EntityBase*>(fixture->GetBody()->GetUserData()));
				return true;
			}
		};


		struct QueryWallByRayCast : public b2RayCastCallback
		{
			std::vector<b2Body*> walls;

			virtual float32 ReportFixture(
				b2Fixture* fixture,
				const b2Vec2& point,
				const b2Vec2& normal,
				float32 fraction) override
			{
				if (fixture->GetBody()->GetUserData() == nullptr)
					walls.push_back(fixture->GetBody());
				return true;
			}
		};

		struct QueryEntityByRayCast : public b2RayCastCallback
		{
			std::vector<EntityBase*> entities;

			virtual float32 ReportFixture(
				b2Fixture* fixture,
				const b2Vec2& point,
				const b2Vec2& normal,
				float32 fraction) override
			{
				if (fixture->GetBody()->GetUserData())
					entities.push_back(static_cast<EntityBase*>(fixture->GetBody()->GetUserData()));
				return true;
			}
		};


		/// This is a test of typical character collision scenarios. This does not
		/// show how you should implement a character in your application.
		/// Instead this is used to test smooth collision on edge chains.
		class PhysicsManager : public b2ContactListener
		{
		public:

			static bool CheckContact(
				const b2Shape* shapeA, int32 indexA,
				const b2Shape* shapeB, int32 indexB,
				const b2Transform& xfA, const b2Transform& xfB,
				b2DistanceOutput& output);

			PhysicsManager(float worldX, float worldY);

			void Step();

			inline b2World* GetPhysicsWorld() const { return _world; }
			inline b2Body* GetGround() const { return _ground; }

			virtual void BeginContact(b2Contact* contact) override;
			virtual void EndContact(b2Contact* contact) override { B2_NOT_USED(contact); }

			virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
			virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override { B2_NOT_USED(contact); B2_NOT_USED(impulse); }

			b2Body* CreateApplyForceBody(float x, float y, b2Shape* shape);
			b2Body* CreateBody(float x, float y, b2BodyType type, b2Shape* shape, bool sensor);

			std::vector<EntityBase*> queryEntitiesAABB(
				const cocos2d::Vec2& pos,
				float halfWidth, 
				float halfHeight) const;

			std::vector<b2Body*> queryWallsAABB(
				const cocos2d::Vec2& pos,
				float halfWidth,
				float halfHeight) const;

			std::vector<EntityBase*> queryEntitiesRayCast(
				const cocos2d::Vec2& start,
				const cocos2d::Vec2& finish) const;

			std::vector<b2Body*> queryWallsRayCast(
				const cocos2d::Vec2& start,
				const cocos2d::Vec2& finish) const;

			void RemoveBody(b2Body* body);

			DestructionListener _destruction_listener;
			b2Body* _character;
			b2World* _world;
			b2Body* _ground;
			Settings _settings;

			ContactPoint _points[2048];
			int32 _pointCount;
		};
	}
}









