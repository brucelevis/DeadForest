//
//  RenderingSystem.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 31..
//
//

#include "RenderingSystem.hpp"
#include "SizeProtocol.h"
#include "Terrain.hpp"
#include "EntityBase.hpp"
#include "UiLayer.hpp"
#include "Camera2D.hpp"
#include "Game.hpp"
#include "EffectBase.hpp"
#include "EffectSprite.hpp"
#include "Effects.hpp"
using namespace cocos2d;
using namespace realtrick::client;


RenderingSystem::RenderingSystem(Game* game) : _game(game)
{
}


RenderingSystem::~RenderingSystem()
{
}


RenderingSystem* RenderingSystem::create(Game* game, GameResource* res)
{
    auto ret = new (std::nothrow) RenderingSystem(game);
    if ( ret && ret->init(res) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool RenderingSystem::init(GameResource* res)
{
    if ( !Node::init() )
        return false;
    
    setClippingRegion(cocos2d::Rect(0, 0, GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT));
    
    _gameScreenScale = Vec2(GAME_SCREEN_WIDTH / 1136, GAME_SCREEN_HEIGHT / 640);
    
    _renderNode = Node::create();
    _renderNode->setPosition(Vec2(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2));
    _renderNode->retain();
    
    _effects.pushBack(EffectDeferredRendering::create());
    _effects.pushBack(EffectBlur::create());
    _effects.pushBack(EffectNoise::create());
    _effects.pushBack(EffectOutline::create());
    
    _terrain = Terrain::create(_game);
    _renderNode->addChild(_terrain);
    
    _rt = RenderTexture::create(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT);
    _rt->setPosition(Vec2(GAME_SCREEN_WIDTH/2, GAME_SCREEN_HEIGHT/2));
    _rt->retain();
    
    _rt2 = RenderTexture::create(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT);
    _rt2->setPosition(Vec2(GAME_SCREEN_WIDTH/2, GAME_SCREEN_HEIGHT/2));
    _rt2->retain();
    
    _testEffect = EffectSprite::create("normal_bg.png");
    _testEffect->setPosition(GAME_SCREEN_WIDTH / 2 , GAME_SCREEN_HEIGHT / 2);
    _testEffect->setFlippedY(true);
    _testEffect->setEffect(_effects.front());
    addChild(_testEffect);
    
    _testChild = Sprite::create("HelloWorld.png");
    _testChild->retain();
    
    return true;
}


void RenderingSystem::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    _rt->beginWithClear(0.0, 0.0, 0.0, 0.0);
    _renderNode->setScale(getZoomScale().x, getZoomScale().y);
    for( auto& entity : _renderNode->getChildren() )
    {
        auto ent = static_cast<EntityBase*>(entity);
        ent->setPosition( ent->getWorldPosition() - getCameraPosition() );
    }
    _renderNode->visit();
    _rt->end();
    
    _rt2->beginWithClear(0.0, 0.0, 0.0, 0.0);
    _testChild->visit();
    _rt2->end();
    
    _testEffect->getGLProgramState()->setUniformTexture("u_texture1", _rt->getSprite()->getTexture());
    _testEffect->getGLProgramState()->setUniformTexture("u_texture2", _rt2->getSprite()->getTexture());
    
    ClippingRectangleNode::visit(renderer, transform, flags);
}


void RenderingSystem::updateChunk(Camera2D* camera)
{
    _terrain->updateChunk(camera->getCameraPos());
}


void RenderingSystem::addEntity(EntityBase* entity, int zOrder)
{
    _renderNode->addChild(entity, zOrder);
}


void RenderingSystem::removeEntity(EntityBase* entity)
{
    entity->removeFromParentAndCleanup(true);
}


void RenderingSystem::addUINode(cocos2d::Node* node)
{
    this->addChild(node, 100);
}


Vec2 RenderingSystem::getCameraPosition() const
{
    return _game->getCamera()->getCameraPos();
}









