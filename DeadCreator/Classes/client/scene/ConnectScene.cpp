//
//  ConnectScene.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 5. 9..
//
//

#define _ENABLE_ATOMIC_ALIGNMENT_FIX

#include "ConnectScene.hpp"
#include "GameServer.hpp"
#include "MainMenu3.hpp"
#include "SizeProtocol.h"
#include "ParamLoader.hpp"
using namespace realtrick::client;
using namespace cocos2d;


ConnectScene::ConnectScene()
{
}


ConnectScene* ConnectScene::create()
{
    auto ret = new (std::nothrow) ConnectScene();
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


Scene* ConnectScene::createScene()
{
    auto scene = Scene::create();
    auto layer = ConnectScene::create();
    scene->addChild(layer);
    return scene;
}


void ConnectScene::update(float dt)
{
    if ( !GameServer::getInstance().isQueueEmpty() )
    {
        Packet* packet;
        GameServer::getInstance().dequeue(packet);
        packet->decode();
        
        switch ( packet->type() )
        {
            case PacketType::LOGIN_SUCCESS:
            {
                auto obj = fpacket::GetPacketLoginSuccess(packet->body());
                UserDefault::getInstance()->setIntegerForKey("pid", obj->pid());
                UserDefault::getInstance()->setStringForKey("id", obj->id()->c_str());
                
                log("<ConnectScene::update> arrived pid: %d", obj->pid());
                log("<ConnectScene::update> arrived userName: %s", obj->id()->c_str());
                
                Director::getInstance()->replaceScene(MainMenu3::createScene());
                
                break;
            }
                
            default:
            {
                log("<ConnectScene::update> Invalid Packet Arrived. (%d)", static_cast<int>(packet->type()));
                break;
            }
        }
        
        CC_SAFE_DELETE(packet);
    }
    
}


void ConnectScene::doConnect()
{
    GameServer::getInstance().connect(Prm.getValueAsString("serverIP"), Prm.getValueAsString("serverPort"));
    this->stopAllActions();
    
    for(auto i = 0 ; i < _progressDots.size(); ++ i)
    {
        _progressDots[i]->stopAllActions();
        _progressDots[i]->runAction(RepeatForever::create(Spawn::create(FadeTo::create(0.25f, 255),
                                                                        Sequence::create(DelayTime::create(i * 0.1f),
                                                                                         ScaleTo::create(0.4f, 0.2f),
                                                                                         ScaleTo::create(0.4f, 0.1f),
                                                                                         DelayTime::create(1.0f - (i * 0.1f)),
                                                                                         nullptr),
                                                                        nullptr)));
    }
    
    runAction(Sequence::create(DelayTime::create(1.0f),
                               CallFunc::create([this]
                                                {
                                                    log("<ConnectScene::update> try connecting...");
                                                }),
                               nullptr));
}


bool ConnectScene::init()
{
    if ( !Layer::init() )
        return false;
    
    _winSize = Size(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT);
    
    this->scheduleUpdate();
    
    _progressDots.resize(11);
    for(auto i = 0 ; i < _progressDots.size(); ++ i)
    {
        _progressDots[i] = Sprite::create("client/ui/dot.png");
        _progressDots[i]->setScale(0.1f);
        _progressDots[i]->setOpacity(0);
        _progressDots[i]->setColor(Color3B::WHITE);
        _progressDots[i]->setPosition(Vec2(_winSize.width / 2.0f - 100.0f + i * 20.0f, _winSize.height * 0.45f));
        addChild(_progressDots[i]);
    }
    
    _retryButton = ui::Button::create("client/ui/retry_n.png", "client/ui/retry_s.png");
    _retryButton->setPosition(Vec2(_winSize / 2.0f));
    _retryButton->setVisible(false);
    _retryButton->setOpacity(0);
    _retryButton->addTouchEventListener([this](Ref* ref, ui::Widget::TouchEventType type)
                                        {
                                            ui::Button* self =  static_cast<ui::Button*>(ref);
                                            if( type == ui::Widget::TouchEventType::ENDED )
                                            {
                                                doConnect();
                                                self->runAction(Sequence::create(FadeTo::create(0.25f, 0),
                                                                                 Hide::create(),
                                                                                 nullptr));
                                            }
                                        });
    addChild(_retryButton);
    
    doConnect();
    
    return true;
}









