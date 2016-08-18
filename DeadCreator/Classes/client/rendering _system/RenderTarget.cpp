//
//  RenderTarget.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 18..
//
//

#include "RenderTarget.hpp"
#include "Camera2D.hpp"
using namespace realtrick::client;
using namespace cocos2d;


RenderTarget* RenderTarget::create(const cocos2d::Size& textureSize)
{
    auto ret = new (std::nothrow) RenderTarget();
    if ( ret && ret->init(textureSize) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool RenderTarget::init(const cocos2d::Size& textureSize)
{
    if ( !cocos2d::Node::init() )
        return false;
    
    _renderTexture = cocos2d::RenderTexture::create(textureSize.width, textureSize.height);
    addChild(_renderTexture);
    
    return true;
}


void RenderTarget::transform(const cocos2d::Vec2& scale, const cocos2d::Vec2& translate)
{
    setScale(scale.x, scale.y);
    for( auto& entity : getChildren() )
    {
        auto ent = static_cast<EntityBase*>(entity);
        ent->setPosition( ent->getWorldPosition() - translate );
    }
}


cocos2d::Texture2D* RenderTarget::getTexture()
{
    _renderTexture->beginWithClear(0.0, 0.0, 0.0, 1.0);
    cocos2d::Node::visit();
    _renderTexture->end();
    
    return _renderTexture->getSprite()->getTexture();
}









