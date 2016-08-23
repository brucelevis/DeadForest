//
//  DeferredRendering.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 19..
//
//

#include "DeferredRendering.hpp"
#include "RenderTarget.hpp"
#include "RenderingSystem.hpp"
#include "Effects.hpp"
#include "EntityBase.hpp"
using namespace realtrick::client;


DeferredRendering::DeferredRendering()
{
}


DeferredRendering::~DeferredRendering()
{
    for ( const auto& target: _renderTargets ) CC_SAFE_RELEASE(target.second);
    _renderTargets.clear();
}


DeferredRendering* DeferredRendering::create(const std::string& basicTextureName)
{
    auto ret = new (std::nothrow) DeferredRendering();
    if ( ret && ret->initWithFile(basicTextureName) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool DeferredRendering::initWithFile(const std::string& basicTextureName)
{
    if ( !EffectSprite::initWithFile(basicTextureName) )
        return false;
    
    setFlippedY(true);
    setEffect(EffectDeferredRendering::create());
    
    // reserved render targets
    _renderTargets.insert( {"u_staticTex", RenderTarget::create(getContentSize())} );
    _renderTargets.insert( {"u_dynamicTex", RenderTarget::create(getContentSize())} );
    _renderTargets.insert( {"u_occlusionTex", RenderTarget::create(getContentSize())} );
    
    for( const auto& renderTarget : _renderTargets )
    {
        renderTarget.second->setPosition(getContentSize() / 2);
        renderTarget.second->retain();
    }
    
    return true;
}


void DeferredRendering::prepareToRender(const cocos2d::Vec2& zoomScale, const cocos2d::Vec2& cameraPos)
{
    for ( const auto& target : _renderTargets )
    {
        target.second->transform(zoomScale, cameraPos);
    }
    
    getGLProgramState()->setUniformTexture("u_staticTex", _renderTargets["u_staticTex"]->getTexture());
    getGLProgramState()->setUniformTexture("u_dynamicTex", _renderTargets["u_dynamicTex"]->getTexture());
    getGLProgramState()->setUniformTexture("u_occlusionTex", _renderTargets["u_occlusionTex"]->getTexture());
    
    /*_renderTargets["u_dynamicTex"]->enableNormal();
    getGLProgramState()->setUniformTexture("u_normalTex", _renderTargets["u_dynamicTex"]->getTexture());
    _renderTargets["u_dynamicTex"]->disableNormal();*/
}


void DeferredRendering::addEntity(const std::string& renderTargetName, EntityBase* node, int zOrder)
{
    // exception!!
    if ( _renderTargets.count(renderTargetName) == 0 ) throw std::runtime_error("render target is not exist");
    _renderTargets[renderTargetName]->addChild(node, zOrder);
}









