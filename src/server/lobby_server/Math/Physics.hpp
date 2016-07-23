#pragma once

#include "CCMathBase.h"
#include "Geometry.hpp"

namespace realtrick
{
    
    namespace physics
    {
        
		bool     intersect(const geometry::Rect& rect1, const geometry::Rect& rect2);
        
        bool     intersect(const geometry::Rect& rect, const geometry::Circle& circle);
        
        bool     intersect(const geometry::Rect& rect, const geometry::Segment& segment);
        
        bool     intersect(const geometry::Rect& rect, const geometry::Polygon& polygon);
        
        
        
        
        bool     intersect(const geometry::Circle& circle, const geometry::Rect& rect);
        
        bool     intersect(const geometry::Circle& circle1, const geometry::Circle& circle2);
        
        bool     intersect(const geometry::Circle& circle, const geometry::Segment& segment);
        
        bool     intersect(const geometry::Circle& circle, const geometry::Polygon& polygon);
        
        
        
        
        bool     intersect(const geometry::Segment& segment, const geometry::Rect& rect);
        
        bool     intersect(const geometry::Segment& segment, const geometry::Circle& circle);
        
        bool     intersect(const geometry::Segment& segment1, const geometry::Segment& segment2);
        
        bool     intersect(const geometry::Segment& segment, const geometry::Polygon& polygon);
        
        
        
        
        bool     intersect(const geometry::Polygon& polygon, const geometry::Rect& rect);
        
        bool     intersect(const geometry::Polygon& polygon, const geometry::Circle& circle);
        
        bool     intersect(const geometry::Polygon& polygon, const geometry::Segment& segment);
        
        bool     intersect(const geometry::Polygon& polygon1, const geometry::Polygon& polygon2);
        
        
        
        
        bool     intersectGet(const geometry::Segment& segment1, const geometry::Segment& segment2, float &dist);
        
        bool     intersectGet(const geometry::Segment& segment1, const geometry::Segment& segment2, float &dist, Vec2& point);
        
        
        
        
        bool     isContainPointInDiamond(const Vec2& diamondCenter, float halfLen, const Vec2& p);
        
        float    getAngleFromZero(const Vec2& point);
        
    };
    
    
}

#include "Physics.inl"



