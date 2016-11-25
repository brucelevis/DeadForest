//
//  CountdownTimerView.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 10. 19..
//
//

#include "CountdownTimerView.hpp"
#include "Game.hpp"
using namespace realtrick;
using namespace realtrick::client;
using namespace cocos2d;


CountdownTimerView::CountdownTimerView(Game* game) :
_game(game),
_countdownTimer(-1),
_accumulatedTime(0.0f)
{
}


CountdownTimerView* CountdownTimerView::create(Game* game)
{
    auto ret = new (std::nothrow) CountdownTimerView(game);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool CountdownTimerView::init()
{
    if ( !Node::init() )
        return false;
    
    this->scheduleUpdate();
    
    _hourView = ui::Text::create("00", "fonts/SpecialElite.TTF", 20);
    _hourView->setPosition(Vec2(-40, 0));
    addChild(_hourView);
    
    _hourMinuteSeperator = ui::Text::create(":","", 20);
    _hourMinuteSeperator->setPosition(Vec2(-20, 3));
    addChild(_hourMinuteSeperator);
    
    _minuteView = ui::Text::create("00", "fonts/SpecialElite.TTF", 20);
    _minuteView->setPosition(Vec2(0, 0));
    addChild(_minuteView);
    
    _minuteSecondSeperator = ui::Text::create(":", "", 20);
    _minuteSecondSeperator->setPosition(Vec2(20, 3));
    addChild(_minuteSecondSeperator);
    
    _secondView = ui::Text::create("00", "fonts/SpecialElite.TTF", 20);
    _secondView->setPosition(Vec2(40, 0));
    addChild(_secondView);
    
    return true;
}


void CountdownTimerView::update(float dt)
{
    if ( _game->isPaused() ) return ;
    
    if ( _countdownTimer < 0 )
    {
        if ( isVisible() ) setVisible(false);
        _countdownTimer = -1;
        return ;
    }
    
    _accumulatedTime += dt;
    if ( _accumulatedTime >= 1.0f )
    {
        _countdownTimer --;
        tick();
        _accumulatedTime = 0.0f;
    }
}


void CountdownTimerView::tick()
{
    if ( _countdownTimer < 0 ) return ;
    
    int hours = _countdownTimer / 3600;
    int leftTime = _countdownTimer % 3600;
    int minutes = leftTime / 60;
    int seconds = leftTime % 60;
    
    std::string hoursString;
    if ( hours / 10 == 0) hoursString += '0';
    hoursString += _to_string(hours);
    
    std::string minuteString;
    if ( minutes / 10 == 0) minuteString += '0';
    minuteString += _to_string(minutes);
    
    std::string secondString;
    if ( seconds / 10 == 0) secondString += '0';
    secondString += _to_string(seconds);
    
    _hourView->setString(hoursString);
    _minuteView->setString(minuteString);
    _secondView->setString(secondString);
}


void CountdownTimerView::setCountdownTimer(unsigned int seconds)
{
    // 99:59:59 (359999 seconds) is max
    _countdownTimer = std::min(seconds, 359999u);
    if ( _countdownTimer > 0 ) setVisible(true);
    else setVisible(false);
    
    tick();
}


void CountdownTimerView::addCountdownTimer(unsigned int seconds)
{
    // 99:59:59 (359999 seconds) is max
    if ( _countdownTimer == -1 ) _countdownTimer = 0;
    _countdownTimer = std::min(_countdownTimer + seconds, 359999u);
    if ( _countdownTimer > 0 ) setVisible(true);
    else setVisible(false);
    
    tick();
}


void CountdownTimerView::subtractCountdownTimer(unsigned int seconds)
{
    if ( _countdownTimer == -1 ) _countdownTimer = 0;
    _countdownTimer = std::min(_countdownTimer - seconds, 0u);
    if ( _countdownTimer > 0 ) setVisible(true);
    else setVisible(false);
    
    tick();
}


void CountdownTimerView::resetTimer()
{
    _countdownTimer = -1;
    setVisible(false);
    
    tick();
}








