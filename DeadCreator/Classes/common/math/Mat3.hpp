//
//  Mat3.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 11. 12..
//  
//

#pragma once

#include <vector>

#include "cocos2d.h"

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
        void scale(const cocos2d::Vec2& scale);
        
        void rotate(float radian);
        void rotate(const cocos2d::Vec2& heading, const cocos2d::Vec2& side);
        
        void translate(const cocos2d::Vec2& trans);
        
        void inverse();
        
        cocos2d::Vec2 getTransformedVector(const cocos2d::Vec2& v);
        std::vector<cocos2d::Vec2> getTransformedVector(std::vector<cocos2d::Vec2> v);
        
		static cocos2d::Vec2 pointToWorldSpace(
			const cocos2d::Vec2 &point,
			const cocos2d::Vec2 &AgentHeading,
			const cocos2d::Vec2 &AgentSide,
			const cocos2d::Vec2 &AgentPosition);

    private:
        
        void copyFrom(const Mat3& copy);
        void _multiply(const Mat3& rhs);
        
    };
    
    
}








