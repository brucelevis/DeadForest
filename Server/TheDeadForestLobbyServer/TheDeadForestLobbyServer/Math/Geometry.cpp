
#include "Geometry.hpp"

namespace realtrick
{
	namespace geometry
	{
		Rect::Rect()
		{
			setRect(0.0, 0.0, 0.0, 0.0);
		}

		Rect::Rect(float x, float y, float width, float height)
		{
			setRect(x, y, width, height);
		}

		Rect::Rect(const Vec2& pos, float width, float height)
		{
			setRect(pos.x, pos.y, width, height);
		}

		Rect::Rect(const Rect& copy)
		{
			setRect(copy.origin.x, copy.origin.y, copy.width, copy.height);
		}

		Rect& Rect::operator=(const Rect& rhs)
		{
			if (&rhs == this)
				return *this;
			setRect(rhs.origin.x, rhs.origin.y, rhs.width, rhs.height);
			return *this;
		}

		void Rect::setRect(float x, float y, float width, float height)
		{
			origin.x = x;
			origin.y = y;
			this->width = width;
			this->height = height;
		}

		float Rect::getMinX() const
		{
			return origin.x;
		}

		float Rect::getMidX() const
		{
			return origin.x + width / 2.0;
		}

		float Rect::getMaxX() const
		{
			return origin.x + width;
		}

		float Rect::getMinY() const
		{
			return origin.y;
		}

		float Rect::getMidY() const
		{
			return origin.y + height / 2.0;
		}

		float Rect::getMaxY() const
		{
			return origin.y + height;
		}

		bool Rect::equals(const Rect& rect) const
		{
			return (origin == rect.origin && width == rect.width && height == rect.height);
		}

		bool Rect::containPoint(const Vec2& point) const
		{
			return (point.x >= getMinX() && point.x <= getMaxX() && point.y >= getMinY() && point.y <= getMaxY());
		}

		const Rect Rect::ZERO = Rect(0.0, 0.0, 0.0, 0.0);




		//
		// Circle
		//
		Circle::Circle()
		{
			setCircle(0.0, 0.0, 0.0);
		}

		Circle::Circle(float x, float y, float r)
		{
			setCircle(x, y, r);
		}

		Circle::Circle(const Vec2& origin, float r)
		{
			setCircle(origin.x, origin.y, r);
		}

		Circle::Circle(const Circle& copy)
		{
			setCircle(copy.origin.x, copy.origin.y, copy.radius);
		}

		Circle& Circle::operator=(const Circle& rhs)
		{
			if (this == &rhs)
				return *this;
			setCircle(rhs.origin.x, rhs.origin.y, rhs.radius);
			return *this;
		}

		void Circle::setCircle(float x, float y, float r)
		{
			this->origin.x = x;
			this->origin.y = y;
			this->radius = r;
		}

		Circle Circle::getTranslatedCircle(const Vec2& dir, float distance)
		{
			return Circle(origin.x + dir.x * distance, origin.y + dir.y * distance, radius);
		}

		bool Circle::containPoint(const Vec2& point) const
		{
			return (origin.getDistanceSq(point) <= radius * radius);
		}

		const Circle Circle::ZERO = Circle(Vec2::ZERO, 0.0f);



		//
		// Segment
		//
		Segment::Segment()
		{
			setSegment(0.0, 0.0, 0.0, 0.0);
		}

		Segment::Segment(float sx, float sy, float ex, float ey)
		{
			setSegment(sx, sy, ex, ey);
		}

		Segment::Segment(const Vec2& start, const Vec2& end)
		{
			setSegment(start.x, start.y, end.x, end.y);
		}

		Segment::Segment(const Segment& copy)
		{
			setSegment(copy.start.x, copy.start.y, copy.end.x, copy.end.y);
		}

		Segment& Segment::operator=(const Segment& rhs)
		{
			if (this == &rhs)
				return *this;
			setSegment(rhs.start.x, rhs.start.y, rhs.end.x, rhs.end.y);
			return *this;
		}

		void Segment::setSegment(float sx, float sy, float ex, float ey)
		{
			this->start.x = sx;
			this->start.y = sy;
			this->end.x = ex;
			this->end.y = ey;
		}

		Vec2 Segment::getDirection() const
		{
			return (end - start).getNormalized();
		}

		float Segment::getDistance() const
		{
			return start.getDistance(end);
		}

		float Segment::getDistanceSq() const
		{
			return start.getDistanceSq(end);
		}

		// TODO
		bool Segment::containPoint(const Vec2& point) const
		{
			return false;
		}



		//
		// Polygon
		//
		Polygon::Polygon()
		{}

		Polygon::Polygon(const std::vector<Vec2>& segs)
		{
			setPolygon(segs);
		}

		Polygon::Polygon(const Polygon& copy)
		{
			setPolygon(copy.vertices);
		}

		Polygon& Polygon::operator=(const Polygon& rhs)
		{
			if (this == &rhs)
				return *this;
			setPolygon(rhs.vertices);
			return *this;

		}

		void Polygon::setPolygon(const std::vector<Vec2>& segs)
		{
			this->vertices.clear();

			for (std::vector<Vec2>::size_type i = 0; i < segs.size(); ++i)
			{
				this->vertices.push_back(segs[i]);
			}
		}

		void Polygon::pushVertex(const Vec2 point)
		{
			this->vertices.push_back(point);
		}

		// TODO
		bool Polygon::containPoint(const Vec2& point) const
		{
			return false;
		}
	}
}















