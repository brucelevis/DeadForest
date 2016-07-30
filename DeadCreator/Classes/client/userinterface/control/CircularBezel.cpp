//
//  CircularBezel.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 1. 21..
//
//

#include "CircularBezel.hpp"
#include "Physics.hpp"
#include "ParamLoader.hpp"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace realtrick::client;


CircularBezel::CircularBezel() :
_bezel(nullptr),
_image(nullptr),
_isTouched(false),
_oldDirection(Vec2::ZERO),
_debugNode(nullptr),
_debugNodeInit(nullptr),
_minTriggerDeg(2.0f),
_maxTriggerDeg(10.0f),
_interpolatedTriggerDeg(10.0f),
_isDebug(false),
_triggerCallback(nullptr),
_minimumTriggerRadius(80.0f),
_maximumTriggerRadius(200.0f),
_precision(2),
_isBezelMode(true),
_currDirection(Vec2::ZERO)
{
}


CircularBezel::~CircularBezel()
{}


CircularBezel* CircularBezel::create(const char* bezelImagePath, cocos2d::ui::Widget::TextureResType type)
{
    CircularBezel* ret = new (std::nothrow)CircularBezel();
    if ( ret && ret->init(bezelImagePath, type) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool CircularBezel::init(const char* bezelImagePath, cocos2d::ui::Widget::TextureResType type)
{
    _debugNode = DrawNode::create();
    _debugNode->setVisible(false);
    addChild(_debugNode);
    
    _debugNodeInit = DrawNode::create();
    _debugNodeInit->setVisible(false);
    addChild(_debugNodeInit);
    
    _bezel = Button::create(bezelImagePath, bezelImagePath, "", type);
    _bezel->setSwallowTouches(false);
    _bezel->setOpacity(0);
    addChild(_bezel);
    
    if( type == ui::Widget::TextureResType::PLIST) _image = Sprite::createWithSpriteFrameName(bezelImagePath);
    else _image = Sprite::create(bezelImagePath);
    addChild(_image);
    
    
    _bezel->addTouchEventListener([&](Ref* ref, Widget::TouchEventType type) {
        
        _debugNode->clear();
        const float kDebugRad  = 120.0f;
        
        Button* self = (Button*)ref;
        Vec2 world = _getWorldPosition();
        
        switch ( type )
        {
            case Widget::TouchEventType::BEGAN:
            {
                Vec2 touch = self->getTouchBeganPosition();
                Vec2 currDirection = (touch - world).getNormalized();
                _currDirection = currDirection;
                
                if( _minimumTriggerRadius * _minimumTriggerRadius > touch.distanceSquared(world) )
                {
                    _isTouched = false;
                    break;
                }
                
                _isTouched = true;
                _oldDirection = (touch - world).getNormalized();
                
                if ( _isBezelMode )
                {
                    _debugNode->drawDot(_oldDirection * kDebugRad, 5.0f, Color4F::RED);
                }
                else
                {
                    if ( _triggerCallback )
                    {
                        _triggerCallback(this, currDirection);
                    }
                    
                    setBezelDirection(_currDirection);
                }
                
                if ( _clickedCallback )
                {
                    _clickedCallback(this, ui::Widget::TouchEventType::BEGAN);
                }
                
                break;
            }
            case Widget::TouchEventType::MOVED:
            {
                Vec2 touch = self->getTouchMovePosition();
                Vec2 currDirection = (touch - world).getNormalized();
                _currDirection = currDirection;
                
                if( _minimumTriggerRadius * _minimumTriggerRadius > touch.distanceSquared(world) )
                {
                    _isTouched = false;
                    break;
                }
                
                if( _isTouched == false )
                {
                    // 안에서 밖으로 터치했을 때
                    _oldDirection = (touch - world).getNormalized();
                    _isTouched = true;
                }
                
                if ( _isBezelMode )
                {
                    _debugNode->drawDot(_oldDirection * kDebugRad, 5.0f, Color4F::RED);
                    _debugNode->drawDot(currDirection * kDebugRad, 5.0f, Color4F::GREEN);
                    _debugNode->drawSegment(_oldDirection * kDebugRad, currDirection * kDebugRad, 2.0f, Color4F::WHITE);
                    
                    
                    float dot = _oldDirection.dot(currDirection);
                    float offsetDeg = MATH_RAD_TO_DEG(acosf(dot));
                    
                    float distanceFromOrigin = touch.distance(world);
                    distanceFromOrigin = clampf(distanceFromOrigin,
                                                _minimumTriggerRadius + 1,
                                                _maximumTriggerRadius - 1);
                    
                    distanceFromOrigin -= _minimumTriggerRadius;
                    
                    float slice = (_maximumTriggerRadius - _minimumTriggerRadius) / _precision;
                    int multiplier = ((int)distanceFromOrigin / slice);
                    
                    float multiplier2 = (float)multiplier / (_precision - 1);
                    if ( _precision == 1) multiplier2 = 0;
                    
                    _interpolatedTriggerDeg = _maxTriggerDeg - (_maxTriggerDeg - _minTriggerDeg) * multiplier2;
                    
                    if ( _interpolatedTriggerDeg < offsetDeg )
                    {
                        int multiplier = -1;
                        if ( _oldDirection.cross(currDirection) > 0 )
                            multiplier = 1;
                        
                        Mat3 rotMat;
                        rotMat.rotate(multiplier * MATH_DEG_TO_RAD(_interpolatedTriggerDeg));
                        _oldDirection = rotMat.getTransformedVector(_oldDirection);
                        _image->setRotation( _image->getRotation() + _interpolatedTriggerDeg * -multiplier );
                        
                        if (_triggerCallback )
                        {
                            float rot = -( _image->getRotation() );
                            _triggerCallback(this, Vec2(cosf(MATH_DEG_TO_RAD(rot)), sinf(MATH_DEG_TO_RAD(rot))));
                        }
                    }
                }
                else
                {
                    if( _triggerCallback )
                    {
                        _triggerCallback(this, currDirection);
                    }
                    
                    setBezelDirection(_currDirection);
                }
                
                if ( _clickedCallback )
                {
                    _clickedCallback(this, ui::Widget::TouchEventType::MOVED);
                }
                
                break;
            }
            case ui::Widget::TouchEventType::ENDED:
            {
                if ( _clickedCallback )
                {
                    _clickedCallback(this, ui::Widget::TouchEventType::ENDED);
                }
                
                _currDirection = Vec2::ZERO;
                
                break;
            }
            case ui::Widget::TouchEventType::CANCELED:
            {
                if ( _clickedCallback )
                {
                    _clickedCallback(this, ui::Widget::TouchEventType::CANCELED);
                }
                
                _currDirection = Vec2::ZERO;
            }
                
            default: break;
        }
        
    });
    
    return true;
}

void CircularBezel::setTriggerRadius(const std::pair<float ,float>& min_max)
{
    _minimumTriggerRadius = min_max.first;
    _maximumTriggerRadius = min_max.second;
    
    _debugNodeInit->clear();
    _debugNodeInit->drawRect(Vec2(-_bezel->getContentSize().width / 2, -_bezel->getContentSize().height / 2),
                             Vec2(_bezel->getContentSize().width / 2, _bezel->getContentSize().height / 2),
                             Color4F::RED);
    _debugNodeInit->drawCircle(Vec2::ZERO, _minimumTriggerRadius, 360.0f, 50, false, Color4F::GRAY);
    _debugNodeInit->drawCircle(Vec2::ZERO, _maximumTriggerRadius, 360.0f, 50, false, Color4F::GRAY);
}


void CircularBezel::setBezelDirection(float angle)
{
    _image->setRotation(-angle);
}


void CircularBezel::setBezelDirection(const cocos2d::Vec2& dir)
{
    _image->setRotation(-_calculateDegree(dir));
}


void CircularBezel::disableBezel()
{
    _image->setOpacity(80);
    _bezel->setTouchEnabled(false);
}


void CircularBezel::enableBezel()
{
    _image->setOpacity(255);
    _bezel->setTouchEnabled(true);
    _bezel->setSwallowTouches(false);
}









