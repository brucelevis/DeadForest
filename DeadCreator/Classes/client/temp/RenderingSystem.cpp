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
#include "Camera2D.hpp"
#include "GameObject.hpp"
#include "UiLayer.hpp"
using namespace cocos2d;
using namespace realtrick::client;


RenderingSystem::RenderingSystem(Game* game) : _game(game)
{
}


RenderingSystem::~RenderingSystem()
{
    CC_SAFE_RELEASE_NULL(_camera);
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
    
    _clipNode = ClippingRectangleNode::create(cocos2d::Rect(0,0, GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT));
    addChild(_clipNode);
    
    auto temp = Node::create();
    temp->setPosition(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2);
    temp->setScale(1.5);
    _clipNode->addChild(temp);
    
    _rootNode = Node::create();
    _rootNode->setPosition(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2);
    _clipNode->addChild(_rootNode);
    
    _camera = Camera2D::create();
    _camera->retain();
    
    _terrain = Terrain::create(this, res);
    temp->addChild(_terrain, 0);
    
    _uiLayer = UiLayer::create(_game);
    _clipNode->addChild(_uiLayer);
    
    return true;
}


cocos2d::Vec2 RenderingSystem::getCameraPosition() const
{
    return _camera->getCameraPos();
}


void RenderingSystem::setCameraPosition(const cocos2d::Vec2& pos)
{
    _camera->setCameraPos(pos);
}


void RenderingSystem::updateChunk()
{
    _terrain->updateChunk(getCameraPosition());
}


void RenderingSystem::addEntity(GameObject* entity, int zOrder)
{
    _rootNode->addChild(entity, zOrder);
}


void RenderingSystem::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
    for( auto& entity : _rootNode->getChildren() )
    {
        auto ent = static_cast<GameObject*>(entity);
        ent->setPosition( ent->getWorldPosition() - getCameraPosition() );
    }
    Node::visit(renderer, transform, flags);
}









