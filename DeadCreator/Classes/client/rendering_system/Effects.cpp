//
//  Effects.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 19..
//
//

#include "Effects.hpp"
#include "EffectSprite.hpp"
using namespace realtrick::client;
using namespace cocos2d;


//
// Effect Blur
//
void EffectBlur::setTarget(EffectSprite *sprite)
{
    if (_glprogramstate == nullptr) return;
    
    Size size = sprite->getTexture()->getContentSizeInPixels();
    _glprogramstate->setUniformVec2("resolution", size);
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
    _glprogramstate->setUniformFloat("blurRadius", _blurRadius);
    _glprogramstate->setUniformFloat("sampleNum", _blurSampleNum);
#endif
}


bool EffectBlur::init(float blurRadius, float sampleNum)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
    initGLProgramState("example_Blur.fsh");
#else
    initGLProgramState("example_Blur_winrt.fsh");
#endif
    _blurRadius = blurRadius;
    _blurSampleNum = sampleNum;
    
    return true;
}


void EffectBlur::setBlurRadius(float radius)
{
    _blurRadius = radius;
}


void EffectBlur::setBlurSampleNum(float num)
{
    _blurSampleNum = num;
}


//
// EffectOutline
//
bool EffectOutline::init()
{
    initGLProgramState("example_Outline.fsh");
    
    Vec3 color(1.0f, 0.2f, 0.3f);
    GLfloat radius = 0.01f;
    GLfloat threshold = 1.75;
    
    _glprogramstate->setUniformVec3("u_outlineColor", color);
    _glprogramstate->setUniformFloat("u_radius", radius);
    _glprogramstate->setUniformFloat("u_threshold", threshold);
    return true;
}

//
// EffectNoise
//
bool EffectNoise::init()
{
    initGLProgramState("example_Noisy.fsh");
    return true;
}

void EffectNoise::setTarget(EffectSprite* sprite)
{
    if (_glprogramstate == nullptr) return;
    
    auto s = sprite->getTexture()->getContentSizeInPixels();
    getGLProgramState()->setUniformVec2("resolution", Vec2(s.width, s.height));
}



//
// Effect Deferred Rendering
//
bool EffectDeferredRendering::init()
{
    initGLProgramState("client/shaders/shader_deferred.fsh");
    return true;
}


void EffectDeferredRendering::setTarget(EffectSprite* sprite)
{
    if (_glprogramstate == nullptr) return;
    
//    auto size = sprite->getTexture()->getContentSizeInPixels();
//    _glprogramstate->setUniformVec2("resolution", size);
//    _glprogramstate->setUniformFloat("blurRadius", 10.0f);
//    _glprogramstate->setUniformFloat("sampleNum", 5.0f);
}



