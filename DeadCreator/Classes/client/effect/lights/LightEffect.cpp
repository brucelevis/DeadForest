//
//  LightEffect.cpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 2. 8..
//
//

#include "LightEffect.hpp"
#include "GameManager.hpp"
using namespace realtrick::client;

LightEffect::LightEffect() : _normalMap(nullptr)
{}


LightEffect::~LightEffect()
{
    CC_SAFE_RELEASE_NULL(_normalMap);
}


LightEffect* LightEffect::create(const std::string& fragment)
{
    LightEffect *ret = new (std::nothrow) LightEffect();
    if (ret && ret->initGLProgramState(fragment))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


void LightEffect::setNormalMap(cocos2d::Texture2D* map)
{
    if( _normalMap != map )
    {
        _normalMap = map;
    }
}


void LightEffect::setDynamicMap(cocos2d::Texture2D* map)
{
    if( _dynamicMap != map )
    {
        _dynamicMap = map;
    }
}


void LightEffect::setStaticMap(cocos2d::Texture2D* map)
{
    if( _staticMap != map )
    {
        _staticMap = map;
    }
}


void LightEffect::setOcculusionMap(cocos2d::Texture2D* map)
{
    if( _occulusionMap != map )
    {
        _occulusionMap = map;
    }
}


void LightEffect::prepareForRender(cocos2d::Sprite *sprite)
{
    auto gl = getGLProgramState();
    gl->setUniformTexture("u_normals", _normalMap);
    gl->setUniformTexture("u_staticMap", _staticMap);
    gl->setUniformTexture("u_dynamicMap", _dynamicMap);
    gl->setUniformTexture("u_occulusionMap", _occulusionMap);
    gl->setUniformVec2("u_contentSize", sprite->getContentSize());
    
    for( int i = 0 ; i < (int)_lights.size() ; ++ i)
    {
        std::string idx = _to_string(i);
        gl->setUniformInt("u_lightType" + idx, _lights.at(i)->getLightType());
        gl->setUniformVec3("u_lightColor" + idx, _lights.at(i)->getColor());
        gl->setUniformFloat("u_isEnable" + idx, _lights.at(i)->isEnable() ? 1.0 : 0.0);
        gl->setUniformFloat("u_intensity" + idx, _lights.at(i)->getIntensity());
        
        switch ( _lights.at(i)->getLightType() )
        {
            case LightBase::LightType::DIRECTIONAL:
            {
                DirectionalLight* l = static_cast<DirectionalLight*>(_lights.at(i));
                gl->setUniformVec3("u_lightDir" + idx, l->getDirection());
                break;
            }
            case LightBase::LightType::POINT:
            {
                PointLight* l = static_cast<PointLight*>(_lights.at(i));
                gl->setUniformVec3("u_lightPos" + idx, l->getLightPos());
                gl->setUniformFloat("u_lightRange" + idx, l->getRange());
                break;
            }
            case LightBase::LightType::SPOT:
            {
                break;
            }
            default: break;
        }
    }
}


void LightEffect::addLight(LightBase* light)
{
    _lights.pushBack(light);
}


void LightEffect::removeLight(LightBase* light)
{
    _lights.eraseObject(light);
}


void LightEffect::removeAllLights()
{
    _lights.clear();
}









