#include "AppDelegate.h"
#include "EditScene.hpp"
#include "DummyScene.hpp"
#include "ShaderTestScene.hpp"

#include "realtrick/imgui/ImGuiGLViewImpl.h"
#include "realtrick/imgui/ImGuiLayer.h"
#include "SizeProtocol.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(SCREEN_WIDTH, SCREEN_HEIGHT);

AppDelegate::AppDelegate() {
    
}

AppDelegate::~AppDelegate()
{
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview)
    {
        glview = ImGuiGLViewImpl::createWithRect("Dead Creator v2.0.0 - Cocos2d-x based GUI Game Development Environment", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
        director->setOpenGLView(glview);
    }
    
    director->setDisplayStats(false);
    director->setAnimationInterval(1.0 / 60);
    
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
    register_all_packages();
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images.plist");
    
    auto scene = realtrick::editor::EditScene::createScene();
    director->runWithScene(scene);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    //
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    //
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    //
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    //
}









