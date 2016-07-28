//
//  DummyScene.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 29..
//
//

#pragma once

#include "cocos2d.h"

#include "SizeProtocol.h"
#include "GameWorld.hpp"

namespace realtrick
{
    namespace client
    {
        
        class DummyScene : public cocos2d::Layer
        {
        
        public:
            
            static DummyScene* create()
            {
                auto ret = new (std::nothrow) DummyScene();
                if ( ret && ret->init() )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            bool init() override
            {
                if ( !Layer::init() )
                    return false;
                
                auto winSize = cocos2d::Size(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT);
                
                auto spr = cocos2d::Sprite::create("HelloWorld.png");
                spr->setPosition( winSize / 2 );
                addChild(spr);
                
                spr->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(2.0f),
                                                         cocos2d::CallFunc::create([this]{
                    
                    _game = GameWorld::create();
                    addChild(_game);
                    
                }), nullptr));

                
                return true;
            }
            
        private:
            
            GameWorld* _game;
            
        };
        
    }
}









