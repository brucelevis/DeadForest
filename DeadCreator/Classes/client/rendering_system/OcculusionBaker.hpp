//
//  OcculusionBaker.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2015. 12. 5..
//
//

#pragma once

#include "ClipperWrapper.hpp"

#define SEGMENT_LENGTH      10000

namespace realtrick
{
    namespace client
    {
        
        class OcculusionBaker : public cocos2d::Ref
        {
            
        public:
            
            struct FieldOfView
            {
                bool isEnable;
                cocos2d::Vec2 heading;
                float entryDegree;
                float aroundCircleRadius;
                int aroundCircleSlice;
                
                FieldOfView() = default;
                virtual ~FieldOfView() = default;
                FieldOfView(bool f, const cocos2d::Vec2& h, float d, float r, int s) :
                isEnable(f),
                heading(h),
                entryDegree(d),
                aroundCircleRadius(r),
                aroundCircleSlice(s)
                {}
            };
            
        public:
            
            enum class WindingOrder { CW, CCW };
            
        public:
            
            OcculusionBaker();
            virtual ~OcculusionBaker();
            
            bool init(const cocos2d::Size& visibleSize);
            static OcculusionBaker* create(const cocos2d::Size& visibleSize);
            
            void setWindingOrder(WindingOrder order)                { _windingOrder = order; }
            void setShadowColor(const cocos2d::Color4F& color)      { _shadowColor = color; }
            void setVisibleColor(const cocos2d::Color4F& color)     { _visibleColor = color; }
            void bakeTexture(cocos2d::RenderTexture* tex,
                             const cocos2d::Vec2& worldPos,
                             const cocos2d::Vec2& scaleFactor,
                             const std::vector<realtrick::Polygon>& polygons,
                             const cocos2d::Size& boundarySize,
                             const FieldOfView& fov);
            
        private:
            
            inline bool _isVisibleWall(const Segment& wall) const;
            inline int  _hashFunc(const cocos2d::Vec2& p) const;
            inline bool _isOneHitVertex(const std::vector<Segment>& walls, const cocos2d::Vec2& vert) const;
            inline cocos2d::Vec2 _getClosestIntersectPointToWall(const std::vector<Segment>& walls, const Segment& seg) const;
            inline cocos2d::Vec2 _worldToLocal(const cocos2d::Vec2& point);
            
        private:
            
            cocos2d::DrawNode*                      _dNode;
            std::vector<realtrick::Polygon>         _polygons;
            cocos2d::Rect                           _boundary;
            cocos2d::Color4F                        _shadowColor;
            cocos2d::Color4F                        _visibleColor;
            WindingOrder                            _windingOrder;
            cocos2d::Vec2                           _eyePos;
            cocos2d::CameraFlag                     _cameraMask;
            FieldOfView                             _fov;

        };
        
    }
}

#include "OcculusionBaker.inl"









