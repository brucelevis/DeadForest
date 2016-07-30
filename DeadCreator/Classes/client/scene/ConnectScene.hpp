//
//  ConnectScene.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 5. 9..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace realtrick
{
    namespace client
    {
        
        class ConnectScene : public cocos2d::Layer
        {
            
        public:
            
            ConnectScene();
            virtual ~ConnectScene() = default;
            virtual bool init() override;
            
            static ConnectScene* create();
            static cocos2d::Scene* createScene();
            
            virtual void update(float dt) override;
            
        private:
            
            void doConnect();
            
        private:
            
            cocos2d::Size _winSize;
            std::vector<cocos2d::Sprite*> _progressDots;
            cocos2d::ui::Button* _retryButton;
            
        };
        
    }
}









