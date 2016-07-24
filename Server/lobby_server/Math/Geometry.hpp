#pragma once

#include <vector>
#include <chrono>

#include "Mat3.hpp"
#include "Vec2.h"

namespace realtrick
{

	namespace geometry
	{

		class Rect
		{

		public:

            Vec2					origin;

			float                   width;

			float                   height;

			Rect();

			Rect(float x, float y, float width, float height);

			Rect(const Vec2& pos, float width, float height);

			Rect(const Rect& copy);

			Rect& operator=(const Rect& rhs);

			void setRect(float x, float y, float width, float height);

			float getMinX() const;

			float getMidX() const;

			float getMaxX() const;

			float getMinY() const;

			float getMidY() const;

			float getMaxY() const;

			bool equals(const Rect& rect) const;

			bool containPoint(const Vec2& point) const;

			static const Rect ZERO;

			virtual ~Rect() {}

		};

		class Circle
		{

		public:

            Vec2			        origin;

			float                   radius;

			Circle();

			Circle(float x, float y, float r);

			Circle(const Vec2& origin, float r);

			Circle(const Circle& copy);

			Circle& operator=(const Circle& rhs);

			void setCircle(float x, float y, float r);

			Circle getTranslatedCircle(const Vec2& dir, float distance);

			bool containPoint(const Vec2& point) const;

			static const Circle ZERO;

			virtual ~Circle() {}

		};



		class Segment
		{

		public:

			Vec2		         start;

			Vec2		         end;

			Segment();

			Segment(float sx, float sy, float ex, float ey);

			Segment(const Vec2& start, const Vec2& end);

			Segment(const Segment& copy);

			Segment& operator=(const Segment& rhs);

			void setSegment(float sx, float sy, float ex, float dy);

			Vec2 getDirection() const;

			float getDistance() const;

			float getDistanceSq() const;

			bool containPoint(const Vec2& point) const;

			virtual ~Segment() {}
		};

		class Polygon
		{

		public:

			std::vector<Vec2> vertices;

			Polygon();

			Polygon(const std::vector<Vec2>& segs);

			Polygon(const Polygon& copy);

			Polygon& operator=(const Polygon& rhs);

			void setPolygon(const std::vector<Vec2>& segs);

			void pushVertex(const Vec2 point);

			bool containPoint(const Vec2& point) const;

			virtual ~Polygon() { vertices.clear(); }

		};


		inline bool isEqual(float a, float b)
		{
			if (fabs(a - b) < 1E-12)
			{
				return true;
			}

			return false;
		}

		inline bool IsZero(float val)
		{
			return (-std::numeric_limits<float>::min() < val && val < std::numeric_limits<float>::min());
		}
	}
}








