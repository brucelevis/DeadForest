//
//  MinimapLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 21..
//
//

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
    
    _defaultImage = Sprite::create("bg.png");
    _defaultImage->setAnchorPoint(Vec2::ZERO);
    _defaultImage->setScale(SizeProtocol::MINIMAP_SIZE / _defaultImage->getContentSize().width,
                            SizeProtocol::MINIMAP_SIZE / _defaultImage->getContentSize().height);
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


void MinimapLayer::onCenterView(const cocos2d::Vec2& params)
{
    if ( !_gmxLayer ) return ;
    
    auto layerSize = Size(SizeProtocol::MINIMAP_SIZE, SizeProtocol::MINIMAP_SIZE);
    
    _focusWindowRenderer->setPosition(Vec2(_focusWindowSize.width / 2 + params.x * (layerSize.width - _focusWindowSize.width),
                                           _focusWindowSize.height / 2 + params.y * (layerSize.height - _focusWindowSize.height)));
}


void MinimapLayer::setDefaultImage(const std::string& fileName)
{
    _defaultImage->setTexture(fileName);
}


void MinimapLayer::setGMXLayer(GMXLayer *layer)
{
    _gmxLayer = layer;
    Size worldSize = _gmxLayer->getWorldSize();
    
    auto layerSize = Size(SizeProtocol::MINIMAP_SIZE, SizeProtocol::MINIMAP_SIZE);
    
    _focusWindowSize = Size(layerSize.width * (layer->getClippingRegion().size.width / worldSize.width),
                            layerSize.height * (layer->getClippingRegion().size.height / worldSize.height));
    
    _focusWindowRenderer->clear();
    _focusWindowRenderer->drawRect(-Vec2(_focusWindowSize.width / 2, _focusWindowSize.height / 2),
                                   Vec2(_focusWindowSize.width / 2, _focusWindowSize.height / 2), Color4F::WHITE);
    
    _focusWindowRenderer->setPosition(Vec2(_focusWindowSize.width / 2, _focusWindowSize.height / 2));
}

void MinimapLayer::onResize()
{
    auto layerSize = Size(SizeProtocol::MINIMAP_SIZE, SizeProtocol::MINIMAP_SIZE);
    
    _defaultImage->setScale(SizeProtocol::MINIMAP_SIZE / _defaultImage->getContentSize().width,
                            SizeProtocol::MINIMAP_SIZE / _defaultImage->getContentSize().height);

    
    setPosition(Vec2(SizeProtocol::WINDOW_PADDING, _director->getVisibleSize().height - SizeProtocol::MENUBAR_HEIGHT - layerSize.height - SizeProtocol::WINDOW_PADDING));

    if ( _gmxLayer )
    {
        Size worldSize =_gmxLayer->getWorldSize();
        _focusWindowRenderer->clear();
        _focusWindowSize = Size(layerSize.width * (_gmxLayer->getClippingRegion().size.width / worldSize.width),
                                layerSize.height * (_gmxLayer->getClippingRegion().size.height / worldSize.height));
        
        _focusWindowRenderer->drawRect(-Vec2(_focusWindowSize.width / 2, _focusWindowSize.height / 2),
                                       Vec2(_focusWindowSize.width /2, _focusWindowSize.height / 2), Color4F::WHITE);
    }
}










