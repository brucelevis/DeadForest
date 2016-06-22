//
//  MinimapLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 21..
//
//

#include "CCImGui.h"
#include "MinimapLayer.hpp"
#include "GMXLayer.hpp"
#include "SizeProtocol.h"
using namespace cocos2d;

MinimapLayer::MinimapLayer()
{
}


MinimapLayer::~MinimapLayer()
{
}


bool MinimapLayer::init(const cocos2d::Size& layerSize)
{
    if ( !Node::init() )
        return false;
    
    _layerSize = layerSize;
    
    _defaultImage = Sprite::create("bg.png");
    _defaultImage->setAnchorPoint(Vec2::ZERO);
    addChild(_defaultImage);
    
    _focusWindowRenderer = DrawNode::create();
    addChild(_focusWindowRenderer);
    
    return true;
}


MinimapLayer* MinimapLayer::create(const cocos2d::Size& layerSize)
{
    auto ret = new (std::nothrow) MinimapLayer();
    if ( ret && ret->init(layerSize) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_FREE(ret);
    return nullptr;
}


void MinimapLayer::centerView(const cocos2d::Vec2& params)
{
    if ( !_gmxLayer ) return ;
    
    _focusWindowRenderer->setPosition(Vec2(_focusWindowSize.width / 2 + params.x * (_layerSize.width - _focusWindowSize.width),
                                           _focusWindowSize.height / 2 + params.y * (_layerSize.height - _focusWindowSize.height)));
}


void MinimapLayer::setLayerSize(const cocos2d::Size& size)
{
    _layerSize = size;
    
    // todo
    // 1. resize layer
    // 2. center view params update
}


void MinimapLayer::setDefaultImage(const std::string& fileName)
{
    _defaultImage->setTexture(fileName);
}


void MinimapLayer::setGMXLayer(GMXLayer *layer)
{
    _gmxLayer = layer;
    _worldSize = _gmxLayer->getWorldSize();
    
    _focusWindowSize = Size(_layerSize.width * (_gmxLayer->getClippingRegion().size.width / _worldSize.width),
                            _layerSize.height * (_gmxLayer->getClippingRegion().size.height / _worldSize.height));
    
    _focusWindowRenderer->drawRect(-Vec2(_focusWindowSize.width / 2, _focusWindowSize.height / 2),
                                   Vec2(_focusWindowSize.width /2, _focusWindowSize.height / 2), Color4F::WHITE);
    
    _focusWindowRenderer->setPosition(Vec2(_focusWindowSize.width / 2, _focusWindowSize.height / 2));
}

void MinimapLayer::onResize()
{
    setPosition(Vec2(WINDOW_PADDING, _director->getVisibleSize().height - MENUBAR_HEIGHT - _layerSize.height - WINDOW_PADDING));
    
    _focusWindowRenderer->clear();
    _focusWindowSize = Size(_layerSize.width * (_gmxLayer->getClippingRegion().size.width / _worldSize.width),
                            _layerSize.height * (_gmxLayer->getClippingRegion().size.height / _worldSize.height));
    
    _focusWindowRenderer->drawRect(-Vec2(_focusWindowSize.width / 2, _focusWindowSize.height / 2),
                                   Vec2(_focusWindowSize.width /2, _focusWindowSize.height / 2), Color4F::WHITE);

}










