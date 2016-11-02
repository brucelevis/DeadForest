#pragma once
//------------------------------------------------------------------------
//
//  Name:		PhysicsDraw.h
//
//  Desc:		Utility class for wrapping SDL graphic tools
//
//  Author:		Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

#include <memory>
#include "cocos2d.h"
#include <Box2D\Box2D.h>
#include <Box2D\Common\b2Draw.h>

namespace realtrick
{
	namespace editor
	{
		class SimulatorLayer;

		class PhysicsDraw : public b2Draw
		{
		public:

			static void staticInit(SimulatorLayer* game);

			static std::unique_ptr<PhysicsDraw> instance;

			virtual ~PhysicsDraw();

			// Box2D drawing functions.
			/// Draw a closed polygon provided in CCW order.
			virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

			/// Draw a solid closed polygon provided in CCW order.
			virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

			/// Draw a circle.
			virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;

			/// Draw a solid circle.
			virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;

			/// Draw a line segment.
			virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

			void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);

			/// Draw a transform. Choose your own length scale.
			/// @param xf a transform.
			virtual void DrawTransform(const b2Transform& xf) override;

			// Helper
			template<typename Point>
			static inline cocos2d::Vec2 toVec(const Point& p) { return cocos2d::Vec2(p.x, p.y); }

			template<typename Point>
			static inline b2Vec2 tob2Vec(const Point& p) { return b2Vec2(p.x, p.y); }

		private:

			PhysicsDraw(SimulatorLayer* simulatorLayer);

			SimulatorLayer* const _simulatorLayer;

		};
	}
}

