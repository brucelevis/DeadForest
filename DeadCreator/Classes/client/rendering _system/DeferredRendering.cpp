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


bool DeferredRendering::init(const Size& size, const std::string& frag)
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
    
    _time = 0.0f;
    _resolution = Vec2(size.width, size.height);
    
    scheduleUpdate();
    
    setContentSize(size);
    setAnchorPoint(Vec2(0.5f, 0.5f));
    
    _renderTargets["u_staticTex"] = RenderTarget::create(size);
    _renderTargets["u_dynamicTex"] = RenderTarget::create(size);
    _renderTargets["u_normalTex"] = RenderTarget::create(size);
    _renderTargets["u_occlusionTex"] = RenderTarget::create(size);
    
    for( const auto& renderTarget : _renderTargets )
    {
        renderTarget.second->setPosition(size);
        renderTarget.second->retain();
    }
    
    return true;
}


void DeferredRendering::update(float dt)
{
    _time += dt;
}


void DeferredRendering::loadShader(const std::string& frag)
{
    auto fileUtiles = FileUtils::getInstance();
    
    // frag
    auto fragmentFilePath = fileUtiles->fullPathForFilename(frag);
    auto fragSource = fileUtiles->getStringFromFile(fragmentFilePath);
    
    auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_vert, fragSource.c_str());
    auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
    setGLProgramState(glprogramstate);
}


void DeferredRendering::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    for ( const auto& target : _renderTargets )
    {
        target.second->transform(_renderingSystem->getZoomScale(), _renderingSystem->getCameraPosition());
        getGLProgramState()->setUniformTexture(target.first, target.second->getTexture());
    }
    
    _customCommand.init(_globalZOrder, transform, flags);
    _customCommand.func = CC_CALLBACK_0(DeferredRendering::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}


void DeferredRendering::onDraw(const cocos2d::Mat4 &transform, uint32_t flags)
{
    cocos2d::Vec2 origin = Vec2::ZERO;
    cocos2d::Node* p = getParent();
    while( p && p->isVisible() )
    {
        origin += p->getPosition();
        p = p->getParent();
    }
    
    float w = _size.width;
    float h = _size.height;
    GLfloat vertices[12] = {0,0, w,0, w,h, 0,0, 0,h, w,h};
    
    auto glProgramState = getGLProgramState();
    glProgramState->setUniformVec2("u_resolution", _resolution);
    glProgramState->setUniformVec2("u_origin", origin);
    glProgramState->setVertexAttribPointer("a_position", 2, GL_FLOAT, GL_FALSE, 0, vertices);
    
    glProgramState->apply(transform);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,6);
}


void DeferredRendering::addNode(const std::string& renderTargetName, EntityBase* node, int zOrder)
{
    // exception!!
    if ( _renderTargets.count(renderTargetName) == 0 ) throw std::runtime_error("render target is not exist");
    
    _renderTargets[renderTargetName]->addChild(node, zOrder);
}


void DeferredRendering::setPosition(const Vec2 &newPosition)
{
    Node::setPosition(newPosition);
    auto position = getPosition();
    auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto retinaFactor = Director::getInstance()->getOpenGLView()->getRetinaFactor();
    _center = Vec2(position.x * frameSize.width / visibleSize.width * retinaFactor, position.y * frameSize.height / visibleSize.height * retinaFactor);
}








