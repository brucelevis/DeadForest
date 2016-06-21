#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GMXLayer;
class MinimapLayer;

class HelloWorld : public cocos2d::LayerColor
{
    
public:
    
    HelloWorld();
    
    virtual ~HelloWorld() = default;
    
    virtual bool init() override;
    
    virtual void update(float dt) override;
    
    CREATE_FUNC(HelloWorld);
    
private:
    
    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    
    void onMouseDown(cocos2d::Event* event);
    
    void onMouseMove(cocos2d::Event* event);
    
    void onMouseUp(cocos2d::Event* event);
    
    void onCenterView();
    
    void onResize();
    
private:

    cocos2d::Size _oldWindowSize;
    
    const float WINDOW_PADDING = 10.0f;
    
    cocos2d::Size _minimapSize;
    
    float _menuBarHeight;
    
    float _statusBarHeight;
    
    cocos2d::Size _workSpaceSize;
    
    GMXLayer* _gmxLayer;
    
    MinimapLayer* _minimapLayer;
    
    cocos2d::Vec2 _centerPosition;
    
    cocos2d::Vec2 _viewSpaceParams;
    
    bool _isKeyPressed[256];
    
    cocos2d::Vec2 _mousePosition;
    
    bool _isMousePressed;
    
};

#endif // __HELLOWORLD_SCENE_H__
