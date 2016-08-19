//
//  EffectBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 19..
//
//

#pragma once

namespace realtrick
{
    namespace client
    {
        
        class EffectSprite;
        
        class EffectBase : public cocos2d::Ref
        {
            
        public:
            
            cocos2d::GLProgramState* getGLProgramState() const { return _glprogramstate; }
            virtual void setTarget(EffectSprite *sprite){}
            
        protected:
            
            bool initGLProgramState(const std::string &fragmentFilename);
            EffectBase();
            virtual ~EffectBase();
            
        protected:
            
            cocos2d::GLProgramState* _glprogramstate;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
            std::string _fragSource;
            cocos2d::EventListenerCustom* _backgroundListener;
#endif
        };
        
    }
}









