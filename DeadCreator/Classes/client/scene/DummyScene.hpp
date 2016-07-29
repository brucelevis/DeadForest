#pragma once

#include "cocos2d.h"

#include "SizeProtocol.h"
#include "GameWorld.hpp"
#include "MainMenu3.hpp"
#include "PlayGameLayer.hpp"

namespace realtrick
{
    
    namespace client
    {
        
        class GameManager;
        
        class DummyScene : public cocos2d::Layer
        {
            
        public:
            
            virtual ~DummyScene()
            {
                _gameLayer->setGameStart(false);
            }
            
            static DummyScene* create(realtrick::editor::PlayGameLayer* layer)
            {
                auto ret = new (std::nothrow) DummyScene();
                if ( ret && ret->init(layer) )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init(realtrick::editor::PlayGameLayer* layer)
            {
                if ( !Layer::init() )
                    return false;
                
                auto winSize = cocos2d::Size(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT);
                
                _gameLayer = layer;
                
                auto spr = cocos2d::Sprite::create("HelloWorld.png");
                spr->setPosition( winSize / 2 );
                spr->setOpacity(0);
                addChild(spr);
                
                spr->runAction(cocos2d::Sequence::create(cocos2d::FadeTo::create(1.0f, 255),
                                                         cocos2d::FadeTo::create(0.5f, 0),
                                                         cocos2d::CallFunc::create([this]{
                    
                    _game = realtrick::client::GameWorld::create();
                    addChild(_game);
                    
                    _gameMgr = _game->getGameManager();
                    _gameLayer->setGameStart(true);
                    
                }),
                                                         cocos2d::RemoveSelf::create(),
                                                         nullptr));
                
                
                return true;
            }
            
            GameManager* getGameManager() const { return _gameMgr; }
            
        private:
            
            GameWorld* _game = nullptr;
            GameManager* _gameMgr = nullptr;
            realtrick::editor::PlayGameLayer* _gameLayer;
            
        };
        
    }
}









