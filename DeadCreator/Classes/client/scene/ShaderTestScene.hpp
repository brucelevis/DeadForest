//
//  ShaderTestScene.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 18..
//
//

#pragma once

#include "cocos2d.h"
#include "JoystickEx.hpp"

namespace realtrick
{
    namespace client
    {
     
        class ShaderTestScene : public cocos2d::Node
        {
          
        public:
            
            ShaderTestScene();
            virtual ~ShaderTestScene() = default;
            
            CREATE_FUNC(ShaderTestScene);
            static cocos2d::Scene* createScene();
            
            virtual bool init() override;
            virtual void update(float dt) override;
            
        private:
            
            cocos2d::Sprite* _human;
            cocos2d::Vec2 _humanHeading = cocos2d::Vec2::UNIT_X;
            float _humanSpeed = 0.0f;
            float _humanRotation = 0.0f;
            JoystickEx* _joystick;
            
        };
    }
}