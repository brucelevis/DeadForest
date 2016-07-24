#pragma once

#include <vector>
#include "Vec2.h"


namespace realtrick
{
    
    class Mat3
    {
        
    public:
        
        float _11, _12, _13;
        float _21, _22, _23;
        float _31, _32, _33;
        
        Mat3();
        Mat3(const Mat3& copy);
        Mat3& operator=(const Mat3& rhs);
        
        void identity();
        
        void scale(float scale);
        
        void scale(float xScale, float yScale);
        
        void scale(const Vec2& scale);
        
        void rotate(float radian);
        
        void rotate(const Vec2& heading, const Vec2& side);
        
        void translate(const Vec2& trans);
        
        Vec2 getTransformedVector(const Vec2& v);
        
        std::vector<Vec2> getTransformedVector(std::vector<Vec2> v);
        
    private:
        
        void _copy(const Mat3& copy);
        
        void _multiply(const Mat3& rhs);
        
    };
    
    
}








