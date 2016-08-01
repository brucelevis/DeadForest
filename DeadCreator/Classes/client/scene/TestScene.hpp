//
//  TestScene.hpp
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
        
        class TestScene : public cocos2d::Layer
        {
            
        public:
            
            TestScene();
            virtual ~TestScene() = default;
            virtual bool init() override;
            
            static TestScene* create();
            static cocos2d::Scene* createScene();
            
            virtual void update(float dt) override;
            
        private:
            
            cocos2d::Size _winSize;
            
        };
        
    }
}









