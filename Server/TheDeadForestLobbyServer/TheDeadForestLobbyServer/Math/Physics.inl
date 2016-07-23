
#include "Physics.hpp"

using std::pair;
using std::vector;

namespace realtrick
{
    
    namespace physics
    {
        inline bool intersect(const geometry::Rect& a, const geometry::Rect& b)
        {
            return (a.getMinX() <= b.getMaxX() && a.getMaxX() >= b.getMinX() &&
                    a.getMinY() <= b.getMaxY() && a.getMaxY() >= b.getMinY());
        }
        
        inline bool intersect(const geometry::Rect& rect, const geometry::Circle& circle)
        {
            float minX = rect.getMinX();
            float minY = rect.getMinY();
            float maxX = rect.getMaxX();
            float maxY = rect.getMaxY();
            
            return (rect.containPoint(circle.origin) ||
                    circle.containPoint(Vec2(minX, minY)) ||
                    circle.containPoint(Vec2(minX, maxY)) ||
                    circle.containPoint(Vec2(maxX, minY)) ||
                    circle.containPoint(Vec2(maxX, maxY)));
        }
        
        inline bool intersect(const geometry::Rect& rect, const geometry::Segment& segment)
        {
            float minX = rect.getMinX();
            float minY = rect.getMinY();
            float maxX = rect.getMaxX();
            float maxY = rect.getMaxY();
            
            if(intersect(geometry::Segment(minX, minY, maxX, minY), segment) ||
               intersect(geometry::Segment(maxX, minY, maxX, maxY), segment) ||
               intersect(geometry::Segment(maxX, maxY, minX, maxY), segment) ||
               intersect(geometry::Segment(minX, maxY, minX, minY), segment))
                return true;
            
            return false;
        }
        
        inline bool intersect(const geometry::Rect& rect, const geometry::Polygon& polygon)
        {
            float minX = rect.getMinX();
            float minY = rect.getMinY();
            float maxX = rect.getMaxX();
            float maxY = rect.getMaxY();
            
            std::vector<geometry::Segment> segs
            {
				geometry::Segment(minX, minY, maxX, minY), // left down -> right down
				geometry::Segment(maxX, minY, maxX, maxY), // right down -> right up
				geometry::Segment(maxX, maxY, minX, maxY), // right up -> left up
				geometry::Segment(minX, maxY, minX, minY)  // left up -> left down
            };
            
            for(std::vector<geometry::Segment>::size_type i = 0 ; i < segs.size() ; ++ i)
            {
                for(std::vector<Vec2>::size_type j = 0; j < polygon.vertices.size() - 1 ; ++ j)
                {
                    if(intersect(segs[i], geometry::Segment(polygon.vertices[j], polygon.vertices[j + 1])))
                        return true;
                }
                if(intersect(segs[i], geometry::Segment(polygon.vertices.back(), polygon.vertices.front())))
                    return true;
            }
            
            return false;
        }
        
        
        
        
        //
        // Circle
        //
        inline bool intersect(const geometry::Circle& circle, const geometry::Rect& rect)
        {
            return intersect(rect, circle);
        }
        
        inline bool intersect(const geometry::Circle& circle1, const geometry::Circle& circle2)
        {
            return (circle1.origin.getDistanceSq(circle2.origin) <=
                    (circle1.radius + circle2.radius) * (circle1.radius + circle2.radius));
        }
        
        inline bool intersect(const geometry::Circle& circle, const geometry::Segment& segment)
        {
            Vec2 d = segment.end - segment.start;
			Vec2 f = segment.start - circle.origin;
            
            float a = d.getLengthSq();
            float b = 2 * f.dot(d);
            float c = f.getLengthSq() - (circle.radius * circle.radius);
            
            float discriminant = b * b - 4.0f * a * c;
            if (discriminant < 0.0f)
            {
                return false;
            }
            else
            {
                discriminant = sqrt(discriminant);
                float t1 = (-b - discriminant) / (2.0f * a);
                float t2 = (-b + discriminant) / (2.0f * a);
                
                if (t1 >= 0.0f && t1 <= 1.0f)
                {
                    return true;
                }
                
                if (t2 >= 0.0f && t2 <= 1.0f)
                {
                    return true;
                }
            }
            
            return false;
        }
        
        inline bool intersect(const geometry::Circle& circle, const geometry::Polygon& polygon)
        {
            for(std::vector<Vec2>::size_type j = 0; j < polygon.vertices.size() - 1 ; ++ j)
            {
                if(intersect(circle, geometry::Segment(polygon.vertices[j], polygon.vertices[j + 1])))
                    return true;
            }
            if(intersect(circle, geometry::Segment(polygon.vertices.back(), polygon.vertices.front())))
                return true;
            
            return false;
        }
        
        
        //
        // Segment
        //
        inline bool intersect(const geometry::Segment& segment, const geometry::Rect& rect)
        {
            return intersect(rect, segment);
        }
        
        inline bool intersect(const geometry::Segment& segment, const geometry::Circle& circle)
        {
            return intersect(circle, segment);
        }
        
        inline bool intersect(const geometry::Segment& segment1, const geometry::Segment& segment2)
        {
            float under =
            (segment2.end.y - segment2.start.y) * (segment1.end.x - segment1.start.x) -
            (segment2.end.x - segment2.start.x) * (segment1.end.y - segment1.start.y);
            
            if (under == 0.0f) return false;
            
            float _t =
            (segment2.end.x - segment2.start.x) * (segment1.start.y - segment2.start.y) -
            (segment2.end.y - segment2.start.y) * (segment1.start.x - segment2.start.x);
            
            float _s =
            (segment1.end.x - segment1.start.x) * (segment1.start.y - segment2.start.y) -
            (segment1.end.y - segment1.start.y) * (segment1.start.x - segment2.start.x);
            
            float t = _t / under;
            float s = _s / under;
            
            if (t < 0.0f || t > 1.0f || s < 0.0f || s > 1.0f) return false;
            if (_t == 0.0f && _s == 0.0f) return false;
            
            return true;
        }
        
        inline bool intersect(const geometry::Segment& segment, const geometry::Polygon& polygon)
        {
            for(std::vector<Vec2>::size_type j = 0; j < polygon.vertices.size() - 1 ; ++ j)
            {
                if(intersect(segment, geometry::Segment(polygon.vertices[j], polygon.vertices[j + 1])))
                    return true;
            }
            if(intersect(segment, geometry::Segment(polygon.vertices.back(), polygon.vertices.front())))
                return true;
            
            return false;
        }
        
        //
        // Polygon
        //
        inline bool intersect(const geometry::Polygon& polygon, const geometry::Rect& rect)
        {
            return intersect(rect, polygon);
        }
        
        inline bool intersect(const geometry::Polygon& polygon, const geometry::Circle& circle)
        {
            return intersect(circle, polygon);
        }
        
        inline bool intersect(const geometry::Polygon& polygon, const geometry::Segment& segment)
        {
            return intersect(segment, polygon);
        }
        
        inline bool intersect(const geometry::Polygon& polygon1, const geometry::Polygon& polygon2)
        {
            
            for(std::vector<Vec2>::size_type i = 0; i < polygon1.vertices.size() - 1 ; ++ i)
            {
                for(std::vector<Vec2>::size_type j = 0; j < polygon2.vertices.size() - 1 ; ++ j)
                {
                    if(intersect(geometry::Segment(polygon1.vertices[i], polygon1.vertices[i + 1]), geometry::Segment(polygon2.vertices[j], polygon2.vertices[j + 1])))
                        return true;
                }
                
                if(intersect(geometry::Segment(polygon1.vertices[i], polygon1.vertices[i + 1]), geometry::Segment(polygon2.vertices.back(), polygon2.vertices.front())))
                    return true;
            }
            
            for(std::vector<Vec2>::size_type i = 0; i < polygon2.vertices.size() - 1 ; ++ i)
            {
                if(intersect(geometry::Segment(polygon1.vertices.back(), polygon1.vertices.front()), geometry::Segment(polygon2.vertices[i], polygon2.vertices[i + 1])))
                    return true;
            }
            
            if(intersect(geometry::Segment(polygon1.vertices.back(), polygon1.vertices.front()), geometry::Segment(polygon2.vertices.back(), polygon2.vertices.front())))
                return true;

            
            return false;
        }
        
        
        inline bool intersectGet(const geometry::Segment& segment1, const geometry::Segment& segment2, float &dist)
        {
            Vec2 A = segment1.start;
			Vec2 B = segment1.end;
			Vec2 C = segment2.start;
			Vec2 D = segment2.end;
            
            float rTop = (A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y);
            float sTop = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);
            
            float Bot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);
            
            
            if (Bot == 0)//parallel
            {
                if (geometry::isEqual(rTop, 0) && geometry::isEqual(sTop, 0))
                {
                    return true;
                }
                return false;
            }
            
            float r = rTop/Bot;
            float s = sTop/Bot;
            
            if( (r > 0) && (r < 1) && (s > 0) && (s < 1) )
            {
                dist = A.getDistance(B) * r;
                
                return true;
            }
            
            else
            {
                dist = 0;
                
                return false;
            }
        }
        
        
        
        inline bool intersectGet(const geometry::Segment& segment1, const geometry::Segment& segment2, float &dist, Vec2& point)
        {
            Vec2 A = segment1.start;
            Vec2 B = segment1.end;
            Vec2 C = segment2.start;
            Vec2 D = segment2.end;
            
            float rTop = (A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y);
            float rBot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);
            
            float sTop = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);
            float sBot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);
            
            if ( (rBot == 0) || (sBot == 0))
            {
                //lines are parallel
                return false;
            }
            
            float r = rTop/rBot;
            float s = sTop/sBot;
            
            if( (r > 0) && (r < 1) && (s > 0) && (s < 1) )
            {
                dist = A.getDistance(B) * r;
                
                point = A + r * (B - A);
                
                return true;
            }
            
            else
            {
                dist = 0;
                
                return false;
            }
        }
        
        inline bool isContainPointInDiamond(const Vec2& diamondCenter, float halfLen, const Vec2& p)
        {
            int m = 1;
            float b[4];

			Vec2 leftPoint = Vec2(diamondCenter.x - halfLen, diamondCenter.y);
			Vec2 rightPoint = Vec2(diamondCenter.x + halfLen, diamondCenter.y);
			Vec2 topPoint = Vec2(diamondCenter.x, diamondCenter.y + halfLen);
			Vec2 botPoint = Vec2(diamondCenter.x, diamondCenter.y - halfLen);

            b[0] = leftPoint.y - m * leftPoint.x;
            b[1] = topPoint.y + m * topPoint.x;
            b[2] = rightPoint.y - m * rightPoint.x;
            b[3] = botPoint.y + m * botPoint.x;
            
            if (p.y - (m * p.x) - b[0] < 0.f
                && p.y - (-m * p.x) - b[1] < 0.f
                && p.y - (m * p.x) - b[2] > 0.f
                && p.y - (-m * p.x) - b[3] > 0.f)
            {
                return true;
            }
            
            return false;
        }
        

        inline float getAngleFromZero(const Vec2& dir)
        {
            if( dir.y >= 0.0f )
            {
                return MATH_RAD_TO_DEG(acosf(dir.dot(Vec2::UNIT_X)));
            }
            return MATH_RAD_TO_DEG(acosf(dir.dot(-Vec2::UNIT_X)) + 3.141592f);
        }
        
        inline float getAngleFromAxis(const Vec2& dir, const Vec2& axis)
        {
            Vec2 a = axis.getNormalized();
            if( a.cross(dir) > 0.0f )
            {
                return MATH_RAD_TO_DEG(acosf(dir.dot(a)));
            }
            return MATH_RAD_TO_DEG(acosf(dir.dot(-a)) + 3.141592f);
        }
        
        
        
    }
}










