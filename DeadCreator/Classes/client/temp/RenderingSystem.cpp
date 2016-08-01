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
    CC_SAFE_DELETE(_camera);
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
    
    _gameScreenScale = Vec2(GAME_SCREEN_WIDTH / 1136, GAME_SCREEN_HEIGHT / 640);
    
    _clipNode = ClippingRectangleNode::create(cocos2d::Rect(0,0, GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT));
    addChild(_clipNode);
    
    _terrainNode = Node::create();
    _terrainNode->setPosition(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2);
    _clipNode->addChild(_terrainNode);
    
    _rootNode = Node::create();
    _rootNode->setPosition(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2);
    _clipNode->addChild(_rootNode);
    
    _camera = new Camera2D();
    
    _terrain = Terrain::create(this, res);
    _terrainNode->addChild(_terrain, 0);
    
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
    _rootNode->setScale(_gameScreenScale.x * _zoomScale, _gameScreenScale.y * _zoomScale);
    _terrainNode->setScale(_gameScreenScale.x * _zoomScale, _gameScreenScale.y * _zoomScale);
    
    for( auto& entity : _rootNode->getChildren() )
    {
        auto ent = static_cast<GameObject*>(entity);
        ent->setPosition( ent->getWorldPosition() - getCameraPosition() );
    }
    Node::visit(renderer, transform, flags);
}









