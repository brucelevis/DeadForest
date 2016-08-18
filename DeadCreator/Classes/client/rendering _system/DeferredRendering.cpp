//
//  DeferredRendering.cpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 18..
//
//

#include "DeferredRendering.hpp"
using namespace realtrick::client;
using namespace cocos2d;


DeferredRendering::DeferredRendering()
{
}


DeferredRendering::~DeferredRendering()
{
}


DeferredRendering* DeferredRendering::create(const cocos2d::Size& size, const std::string& frag)
{
    auto ret = new (std::nothrow) DeferredRendering();
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
    
    spr = Sprite::create("grossini.png");
    spr->retain();
    
    auto rt = RenderTexture::create(1600, 960);
    rt->setPosition(Vec2(800, 480));
    addChild(rt);
    
    rt->begin();
    spr->visit();
    rt->end();
    
    glprogramstate->setUniformTexture("u_texture1", rt->getSprite()->getTexture());
}


void DeferredRendering::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder, transform, flags);
    _customCommand.func = CC_CALLBACK_0(DeferredRendering::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void DeferredRendering::onDraw(const Mat4 &transform, uint32_t flags)
{
    auto glProgramState = getGLProgramState();
    
    float w = _size.width;
    float h = _size.height;
    GLfloat vertices[12] = {0,0, w,0, w,h, 0,0, 0,h, w,h};
    glProgramState->setVertexAttribPointer("a_position", 2, GL_FLOAT, GL_FALSE, 0, vertices);
    
    glProgramState->apply(transform);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,6);
}









