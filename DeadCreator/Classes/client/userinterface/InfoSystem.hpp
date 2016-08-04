//
//  InfoSystem.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 2. 29..
//
//

#pragma once

#include <string>
#include <chrono>

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace realtrick
{
    namespace client
    {
        
        struct InfoWithTime
        {
            std::chrono::duration<double> dispatchTime;
            std::string msg;
            
            InfoWithTime() : dispatchTime(std::chrono::duration<double>::zero()) {}
        };
        
        class InfoSystem : public cocos2d::Node
        {
            
        public:
            
            InfoSystem();
            virtual ~InfoSystem();
            
            bool init(const std::string& fontPath, float fontSize, const cocos2d::Color3B& color, int viewLine);
            static InfoSystem* create(const std::string& fontPath = "",
                                      float fontSize = 12,
                                      const cocos2d::Color3B& color = cocos2d::Color3B::WHITE,
                                      int viewLine = 3);
            
            void pushMessage(const std::string& msg);
            void setDeathTime(float t) { _deathTime = t; }
            void fontOffset(float offset) { _fontOffset = offset; }
            
        private:
            
            void discharge(const std::string& msg);
            void update(float dt) override;
            
        private:
            
            float                                               _disableTime;
            std::string                                         _fontFilePath;
            float                                               _fontSize;
            cocos2d::Color3B                                    _fontColor;
            std::list<cocos2d::ui::Text*>                       _lines;
            int                                                 _maxLine;
            float                                               _fontOffset;
            std::priority_queue<InfoWithTime>                   _pq;
            std::chrono::duration<double>                       _lastTime;
            float                                               _deathTime;
            
        };
        
        inline bool operator<(const InfoWithTime& info1, const InfoWithTime& info2)
        {
            return  (info1.dispatchTime > info2.dispatchTime);
        }
        
    }
}









