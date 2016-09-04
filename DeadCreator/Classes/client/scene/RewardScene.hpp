#pragma once

#include "cocos2d.h"
#include "SizeProtocol.h"
#include "MainMenu3.hpp"

namespace realtrick
{
    namespace client
    {
        
        class RewardScene : public cocos2d::Node
        {
            
        public:
            
            static RewardScene* create()
            {
                auto ret = new (std::nothrow) RewardScene();
                if ( ret && ret->init() )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            static cocos2d::Scene* createScene()
            {
                auto scene = cocos2d::Scene::create();
                auto node = RewardScene::create();
                scene->addChild(node);
                return scene;
            }
            
            virtual bool init() override
            {
                if ( !Node::init() )
                    return false;
                
                auto winSize = cocos2d::Size(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT);
                
                std::string fileName;
                if ( cocos2d::UserDefault::getInstance()->getBoolForKey("isVictory", false) )
                {
                    fileName = "client/ui/you_win.png";
                }
                else
                {
                    fileName = "client/ui/you_lose.png";
                }
                
                auto spr = cocos2d::Sprite::create(fileName);
                spr->setPosition(winSize / 2.0f);
                addChild(spr);
                
                spr->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(1.0f),
                                                         cocos2d::CallFunc::create([this]{
                    
                    cocos2d::Director::getInstance()->replaceScene(MainMenu3::createScene());
                    
                }),
                                                         nullptr));
                
                return true;
            }
            
        };
        
    }
}









