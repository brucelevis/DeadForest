//
//  MainMenu.cpp
//  TheDeadForest
//
//  Created by 남준현 on 2015. 11. 29..
//
//

#include "MainMenu.hpp"
#include "GameWorld.hpp"
#include "GameManager.hpp"

namespace  realtrick
{
    
    using namespace cocos2d;
    
    cocos2d::Scene* MainMenu::createScene()
    {
        auto scene = Scene::create();
        auto layer = MainMenu::create();
        scene->addChild(layer);
        return scene;
    }
    
    
    MainMenu::MainMenu() :
    queueText(nullptr),
    queueBackBt(nullptr),
    currQueueUser(0),
    _singleBt(nullptr),
    _multiBt(nullptr),
    _exitBt(nullptr),
    _connectInfo(nullptr),
    _version(nullptr),
    _idText(nullptr),
    _userID(""),
    _isRegist(false),
    _isNetworkReady(false),
    _networkAccumulateTime(0.0f),
    _isLoading(false),
    _isSingleReady(false),
    _singleAccumulateTime(0.0f),
    _isConnect(false)
    {
    }
    
    
    MainMenu::~MainMenu()
    {
        for(int i = 0 ; i < _dots.size() ; ++ i)
        {
            _dots[i]->stopAllActions();
        }
    }
    
    
    
    bool MainMenu::init()
    {
        if ( !Layer::init() )
        {
            return false;
        }
        
        this->schedule(schedule_selector(MainMenu::update), 0.03f);
        
        _winSize = Director::getInstance()->getVisibleSize();
        
        
        _title = Sprite::create("game_title.png");
        _title->setPosition(Vec2(_winSize.width * 0.5f, _winSize.height * 0.8f));
        _title->setScale(2.0f);
        _title->setOpacity(0);
        _title->runAction(FadeTo::create(1.0f, 255));
        addChild(_title);
        
        _singleBt = ui::Button::create("single_n.png", "single_n.png");
        _singleBt->setPosition(Vec2(_winSize.width * 0.5f, _winSize.height * 0.55f));
        _singleBt->setOpacity(0);
        _singleBt->setTouchEnabled(false);
        _singleBt->runAction(Sequence::create(DelayTime::create(0.25f), FadeTo::create(1.0f, 255), CallFunc::create([&]{ _singleBt->setTouchEnabled(true); }), nullptr));
        _singleBt->setScale(0.7f);
        _singleBt->addTouchEventListener([this](Ref* ref, ui::Widget::TouchEventType type){
            
            if ( type == ui::Widget::TouchEventType::BEGAN )
            {
                _prepareSingleMode();
                experimental::AudioEngine::play2d("ui_enter.mp3");
            }
            
        });
        addChild(_singleBt);
        
        _multiBt = ui::Button::create("multi_n.png", "multi_n.png");
        _multiBt->setPosition(Vec2(_winSize.width * 0.5f, _winSize.height * 0.45f));
        _multiBt->setOpacity(0);
        _multiBt->setTouchEnabled(false);
        _multiBt->runAction(Sequence::create(DelayTime::create(0.25f), FadeTo::create(1.0f, 255), CallFunc::create([&]{ _multiBt->setTouchEnabled(true); }), nullptr));
        _multiBt->setScale(0.7f);
        _multiBt->addTouchEventListener([this](Ref* ref, ui::Widget::TouchEventType type){
            
            if ( type == ui::Widget::TouchEventType::BEGAN )
            {
                _prepareMultiMode();
                experimental::AudioEngine::play2d("ui_enter.mp3");
            }
            
        });
        addChild(_multiBt);
        
        _exitBt = ui::Button::create("exit_n.png", "exit_n.png");
        _exitBt->setOpacity(0);
        _exitBt->setTouchEnabled(false);
        _exitBt->runAction(Sequence::create(DelayTime::create(0.25f), FadeTo::create(1.0f, 255), CallFunc::create([&]{ _exitBt->setTouchEnabled(true); }), nullptr));
        _exitBt->setScale(0.7f);
        _exitBt->setPosition(Vec2(_winSize.width * 0.5f, _winSize.height * 0.35f));
        _exitBt->addTouchEventListener([this](Ref* ref, ui::Widget::TouchEventType type){
            
            if ( type == ui::Widget::TouchEventType::BEGAN )
            {
                Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                exit(0);
#endif
            }
            
        });
        addChild(_exitBt);
        
        
        _version = ui::Text::create("ver 1.1", "SpecialElite.TTF", 25);
        _version->setOpacity(0);
        _version->runAction(Sequence::create(DelayTime::create(0.25f), FadeTo::create(1.0f, 255), nullptr));
        _version->setPosition(Vec2(_winSize.width - 150, 50));
        addChild(_version);
        
        
        const int SIZE = 18;
        const float RAD = 15.0f;
        _dots.resize(SIZE);
        
        for( int i = 0 ; i < SIZE ; ++ i )
        {
            _dots[i] = Sprite::create("dot.png");
            _dots[i]->setPosition(Vec2(_winSize.width * 0.5f + cosf(MATH_DEG_TO_RAD(i * 360 / SIZE)) * RAD,
                                       _winSize.height * 0.2f + sinf(MATH_DEG_TO_RAD(i * 360 / SIZE)) * RAD));
            _dots[i]->setOpacity((float)i / (SIZE - 1) * 255);
            _dots[i]->setScale(0.10f);
            _dots[i]->setVisible(false);
            addChild(_dots[i]);
        }
        
        _connectInfo = ui::Text::create("Connecting... Please wait.", "SpecialElite.TTF", 20);
        _connectInfo->setPosition(Vec2(_winSize.width * 0.5f, _winSize.height * 0.13f));
        _connectInfo->setVisible(false);
        addChild(_connectInfo);
        
        return true;
    }
    
    
    //
    // Update
    //
    void MainMenu::update(float dt)
    {
        if ( _isLoading )
        {
            int size = (int)_dots.size();
            for(int i = 0 ; i < size; ++ i)
            {
                int op = (int)(_dots[i]->getOpacity() + (1.0f / (size - 1)) * 255) % 255;
                _dots[i]->setOpacity(op);
            }
        }
        
        if ( _isSingleReady )
        {
            const float MAX_TOLERANCE_TIME = 1.0f;
            if ( _singleAccumulateTime < MAX_TOLERANCE_TIME )
            {
                _singleAccumulateTime += dt;
            }
            else
            {
                _singleAccumulateTime = 0.0f;
                _isSingleReady = false;
                
                _title->runAction(Sequence::create(FadeTo::create(1.0f, 0), RemoveSelf::create(), nullptr));
                _singleBt->runAction(Sequence::create(FadeTo::create(1.0f, 0), RemoveSelf::create(), nullptr));
                _multiBt->runAction(Sequence::create(FadeTo::create(1.0f, 0), RemoveSelf::create(), nullptr));
                _exitBt->runAction(Sequence::create(FadeTo::create(1.0f, 0), RemoveSelf::create(), nullptr));
                _version->runAction(Sequence::create(FadeTo::create(1.0f, 0), RemoveSelf::create(), nullptr));
                
                auto spr = Sprite::create();
                spr->setPosition(Vec2(_winSize / 2));
                spr->setTextureRect(cocos2d::Rect(0, 0, _winSize.width, _winSize.height));
                spr->setOpacity(0);
                spr->setColor(Color3B::BLACK);
                spr->runAction(Sequence::create(DelayTime::create(2.0f),
                                                FadeTo::create(1.0f, 255),
                                                CallFunc::create([this] {
                    
                    UserDefault::getInstance()->setBoolForKey("isNetwork", false);
                    Director::getInstance()->replaceScene(GameWorld::createScene());
                    
                }),
                                                nullptr));
                addChild(spr, 1);
                
                
                for(int i = 0 ; i < _dots.size() ; ++ i)
                {
                    _dots[i]->runAction(MoveBy::create(1.0f, Vec2(0.0f, _winSize.height * 0.3f)));
                }
            }
        }
        
    }
    
    
    
    
    
    //
    // 싱글모드 준비
    //
    void MainMenu::_prepareSingleMode()
    {
        _enableButtons(false);
        _enableLoading(true);
        _isLoading = true;
        _isSingleReady = true;
    }
    
    
    
    
    
    
    //
    // 멀티모드 준비
    //
    void MainMenu::_prepareMultiMode()
    {
        
    }
    
    
    void MainMenu::_prepareMultiModeStart()
    {
        
    }
    
    
    
    void MainMenu::_prepareMultiModeFail()
    {
        
    }
    
    
    void MainMenu::_enableLoading(bool enable)
    {
        for(int i = 0 ; i < _dots.size() ; ++ i)
        {
            _dots[i]->setVisible(enable);
        }
    }
    
    
    void MainMenu::_enableButtons(bool enable)
    {
        const int DISABLE_OPACITY = 50;
        
        _singleBt->setOpacity(enable ? 255 : DISABLE_OPACITY);
        _singleBt->setTouchEnabled(enable);
        
        _multiBt->setOpacity(enable ? 255 : DISABLE_OPACITY);
        _multiBt->setTouchEnabled(enable);
        
        _exitBt->setOpacity(enable ? 255 : DISABLE_OPACITY);
        _exitBt->setTouchEnabled(enable);
    }
    
    
    void MainMenu::_hideButtons()
    {
        _singleBt->setVisible(false);
        _singleBt->setTouchEnabled(false);
        
        _multiBt->setVisible(false);
        _multiBt->setTouchEnabled(false);
        
        _exitBt->setVisible(false);
        _exitBt->setTouchEnabled(false);
    }
    
    
    void MainMenu::_showButtons()
    {
        _singleBt->setVisible(true);
        _singleBt->setTouchEnabled(true);
        
        _multiBt->setVisible(true);
        _multiBt->setTouchEnabled(true);
        
        _exitBt->setVisible(true);
        _exitBt->setTouchEnabled(true);
    }
    
    
    void MainMenu::_disconnectFromServer()
    {
       
    }
    
    
    
}
