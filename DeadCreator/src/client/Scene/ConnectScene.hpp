//
//  ConnectScene.hpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 5. 9..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace realtrick
{
    
    class ConnectScene : public cocos2d::Layer
    {
        
    public:
        
        static ConnectScene* create();
        
        static cocos2d::Scene* createScene();
        
        virtual bool init() override;
        
        virtual void update(float dt) override;
        
        ConnectScene();
        
        virtual ~ConnectScene() = default;
        
    private:
        
        void doConnect();
        
    private:
        
        cocos2d::Size _winSize;
        
        std::vector<cocos2d::Sprite*> _progressDots;
        
        cocos2d::ui::Button* _retryButton;
        
    };
    
}