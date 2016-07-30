//
//  AnimatedFiniteEntity.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 3. 5..
//
//

#pragma once

#include "cocos2d.h"
#include "CocosGUI.h"
#include "EntityBase.hpp"

namespace realtrick
{
    namespace client
    {
    
        class GameManager;
        
        class AnimatedFiniteEntity : public EntityBase
        {
            
        public:
            
            explicit AnimatedFiniteEntity(GameManager* mgr);
            virtual ~AnimatedFiniteEntity();
            AnimatedFiniteEntity(const AnimatedFiniteEntity& rhs);
            virtual bool init(const std::vector<std::string> frames, float deathTime, cocos2d::ui::Widget::TextureResType type);
            
            void enableNormal(bool enable) override {}
            virtual AnimatedFiniteEntity* clone() const = 0;
            void setInterval(float interval) { _interval = interval; }
            bool handleMessage(const Telegram& msg) override;
            
        protected:
            
            cocos2d::Sprite*                        _base;
            std::vector<std::string>                _frames;
            float                                   _interval;
            float                                   _deathTime;
            int                                     _currIdx;
            cocos2d::ui::Widget::TextureResType     _texType;
            
        };
        
    }
}









