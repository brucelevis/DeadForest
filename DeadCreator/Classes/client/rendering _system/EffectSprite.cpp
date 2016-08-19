//
//  EffectSprite.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 19..
//
//

#include "EffectSprite.hpp"
#include "EffectBase.hpp"
using namespace realtrick::client;
using namespace cocos2d;


EffectSprite::EffectSprite() : _defaultEffect(nullptr)
{
    _effects.reserve(2);
}


EffectSprite::~EffectSprite()
{
    for(auto &tuple : _effects) {
        std::get<1>(tuple)->release();
    }
    CC_SAFE_RELEASE(_defaultEffect);
}


EffectSprite* EffectSprite::create()
{
    EffectSprite *sprite = new (std::nothrow) EffectSprite();
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}


EffectSprite* EffectSprite::create(const std::string& filename)
{
    auto ret = new (std::nothrow) EffectSprite();
    if(ret && ret->initWithFile(filename))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_RELEASE(ret);
    return nullptr;
}


void EffectSprite::setEffect(EffectBase* effect)
{
    if(_defaultEffect != effect)
    {
        effect->setTarget(this);
        
        CC_SAFE_RELEASE(_defaultEffect);
        _defaultEffect = effect;
        CC_SAFE_RETAIN(_defaultEffect);
        
        setGLProgramState(_defaultEffect->getGLProgramState());
    }
}


void EffectSprite::addEffect(EffectBase *effect, ssize_t order)
{
    effect->retain();
    effect->setTarget(this);
    
    _effects.push_back(std::make_tuple(order,effect,QuadCommand()));
    
    std::sort(std::begin(_effects), std::end(_effects), [](const std::tuple<ssize_t,EffectBase*,QuadCommand> &tuple1,
                                                           const std::tuple<ssize_t,EffectBase*,QuadCommand> &tuple2){
        return std::get<0>(tuple1) < std::get<0>(tuple2);
    });
}

void EffectSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
#if CC_USE_CULLING
    // Don't do calculate the culling if the transform was not updated
    _insideBounds = (flags & FLAGS_TRANSFORM_DIRTY) ? renderer->checkVisibility(transform, _contentSize) : _insideBounds;
    
    if(_insideBounds)
#endif
    {
        // negative effects: order < 0
        int idx = 0;
        for( auto& effect : _effects )
        {
            if(std::get<0>(effect) >=0) break;
            auto glProgramState = std::get<1>(effect)->getGLProgramState();
            if (glProgramState)
            {
                QuadCommand &q = std::get<2>(effect);
                q.init(_globalZOrder, _texture->getName(), glProgramState, _blendFunc, &_quad, 1, transform, flags);
                renderer->addCommand(&q);
            }
            idx++;
        }
        
        // normal effect: order == 0
        _trianglesCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, _polyInfo.triangles, transform, flags);
        renderer->addCommand(&_trianglesCommand);
        
        // postive effects: oder >= 0
        for(auto it = std::begin(_effects) + idx; it != std::end(_effects); ++it)
        {
            QuadCommand &q = std::get<2>(*it);
            q.init(_globalZOrder, _texture->getName(), std::get<1>(*it)->getGLProgramState(), _blendFunc, &_quad, 1, transform, flags);
            renderer->addCommand(&q);
            idx++;
        }
    }
}









