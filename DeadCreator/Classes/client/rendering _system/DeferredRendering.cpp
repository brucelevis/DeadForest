//
//  DeferredRendering.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 18..
//
//

#include "DeferredRendering.hpp"
#include "RenderTarget.hpp"
#include "RenderingSystem.hpp"
#include "EntityBase.hpp"
using namespace realtrick::client;
using namespace cocos2d;



DeferredRendering* DeferredRendering::create(RenderingSystem* sys, const cocos2d::Size& size, const std::string& frag)
{
    auto ret = new (std::nothrow) DeferredRendering(sys);
    if ( ret && ret->init(size, frag) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool DeferredRendering::init(const cocos2d::Size& size, const std::string& frag)
{
    if ( !Node::init() )
        return false;
    
    _size = size;
    
    _fragFileName = frag;
#if CC_ENABLE_CACHE_TEXTURE_DATA
    auto listener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom* event){
        this->setGLProgramState(nullptr);
        loadShader("", _fragFileName);
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif
    
    loadShader(_fragFileName);
    
    _renderTargets["u_staticTex"] = RenderTarget::create(size);
    _renderTargets["u_dynamicTex"] = RenderTarget::create(size);
    _renderTargets["u_normalTex"] = RenderTarget::create(size);
    _renderTargets["u_occlusionTex"] = RenderTarget::create(size);
    
    for( const auto& renderTarget : _renderTargets )
    {
        renderTarget.second->setPosition(size / 2);
        renderTarget.second->retain();
//        addChild(renderTarget.second);
    }
    
    return true;
}


void DeferredRendering::loadShader(const std::string &frag)
{
    auto fileUtiles = FileUtils::getInstance();
    
    // vert
    std::string vertSource;
    vertSource = ccPositionTextureColor_vert;
    
    // frag
    auto fragmentFilePath = fileUtiles->fullPathForFilename(frag);
    auto fragSource = fileUtiles->getStringFromFile(fragmentFilePath);
    
    auto glprogram = GLProgram::createWithByteArrays(vertSource.c_str(), fragSource.c_str());
    auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
    setGLProgramState(glprogramstate);
}


void DeferredRendering::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    for ( const auto& target : _renderTargets )
    {
        const auto& textureName = target.first;
        const auto& renderTarget = target.second;
        renderTarget->transform(_renderingSystem->getZoomScale(), _renderingSystem->getCameraPosition());
        this->getGLProgramState()->setUniformTexture(textureName, renderTarget->getTexture());
    }
    
    float w = _size.width;
    float h = _size.height;
    GLfloat vertices[12] = {0,0, w,0, w,h, 0,0, 0,h, w,h};
    this->getGLProgramState()->setVertexAttribPointer("a_position", 2, GL_FLOAT, GL_FALSE, 0, vertices);
    
    _customCommand.init(_globalZOrder, transform, flags);
    _customCommand.func = std::bind(&DeferredRendering::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}


void DeferredRendering::onDraw(const Mat4 &transform, uint32_t flags)
{
    this->getGLProgramState()->apply(transform);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,6);
}


void DeferredRendering::addNode(const std::string& renderTargetName, EntityBase* node, int zOrder)
{
//    if ( _renderTargets.count(renderTargetName) == 0 )
//        throw std::runtime_error("render target name is not exist");

    _renderTargets[renderTargetName]->addChild(node, zOrder);
}









