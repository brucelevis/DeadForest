//
//  CountdownTimerView.hpp
//  DeadCreator
//
//  Created by mac on 2016. 10. 19..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"


namespace realtrick
{
    namespace client
    {
        
        class Game;
        
        class CountdownTimerView : public cocos2d::Node
        {
            
        public:
            
            explicit CountdownTimerView(Game* game);
            virtual ~CountdownTimerView() = default;
            
            static CountdownTimerView* create(Game* game);
            virtual bool init() override;
            virtual void update(float dt) override;
            
            void setCountdownTimer(unsigned int seconds);
            void addCountdownTimer(unsigned int seconds);
            void subtractCountdownTimer(unsigned int seconds);
            void resetTimer();
            
            const int getCountdownTimer() const { return _countdownTimer; }
                                   
        private:
            
            Game* _game;
            int _countdownTimer;
            float _accumulatedTime;
            
            cocos2d::ui::Text* _hourView;
            cocos2d::ui::Text* _minuteView;
            cocos2d::ui::Text* _secondView;
            
            cocos2d::ui::Text* _hourMinuteSeperator;
            cocos2d::ui::Text* _minuteSecondSeperator;
            
        };
    }
}









