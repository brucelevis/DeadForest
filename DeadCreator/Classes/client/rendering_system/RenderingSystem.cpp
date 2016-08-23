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
#include "DeferredRendering.hpp"
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
    
    _deferredRendering = DeferredRendering::create("client/normal_bg.png");
    _deferredRendering->setPosition(Vec2(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2));
    addChild(_deferredRendering);
    
    _terrain = Terrain::create(_game);
    _deferredRendering->addEntity("u_staticTex", _terrain);

    return true;
}


void RenderingSystem::visit(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
     _deferredRendering->prepareToRender(getZoomScale(), getCameraPosition());
    ClippingRectangleNode::visit(renderer, transform, flags);
}


void RenderingSystem::updateChunk(Camera2D* camera)
{
    _terrain->updateChunk(camera->getCameraPos());
}


void RenderingSystem::addEntity(EntityBase* entity, int zOrder)
{
    _deferredRendering->addEntity("u_dynamicTex", entity, zOrder);
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









