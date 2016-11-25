#pragma once

#include "cocos2d.h"

#include "SizeProtocol.h"
#include "Game.hpp"
#include "SimulatorLayer.hpp"

namespace realtrick
{
    
    namespace client
    {
        
        class Game;
        
        class DummyScene : public cocos2d::Layer
        {
            
        public:
            
            virtual ~DummyScene()
            {
                _simulator->setGameStart(false);
            }
            
            static DummyScene* create(realtrick::editor::SimulatorLayer* layer)
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
            
            bool init(realtrick::editor::SimulatorLayer* layer)
            {
                if ( !Layer::init() )
                    return false;
                
                auto winSize = cocos2d::Size(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT);
                
                _simulator = layer;
                
                auto spr = cocos2d::Sprite::create("editor/HelloWorld.png");
                spr->setPosition( winSize / 2 );
                spr->setOpacity(0);
                addChild(spr);
                
                spr->runAction(cocos2d::Sequence::create(cocos2d::FadeTo::create(1.0f, 255),
                                                         cocos2d::FadeTo::create(0.5f, 0),
                                                         cocos2d::CallFunc::create([this]{
                    
                    _game = realtrick::client::Game::createWithSimulator(_simulator);
                    addChild(_game);
                    
                    _simulator->setGameStart(true);
                    
                }),
                                                         cocos2d::RemoveSelf::create(),
                                                         nullptr));
                
                
                return true;
            }
            
            Game* getGame() const { return _game; }
            
        private:
            
            Game* _game = nullptr;
            realtrick::editor::SimulatorLayer* _simulator;
            
        };
        
    }
}









