#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::LayerColor
{
    
public:
    
    HelloWorld();
    
    virtual ~HelloWorld() = default;
    
    virtual bool init() override;
    
    virtual void update(float dt) override;
    
    CREATE_FUNC(HelloWorld);
    
    void loadMap(const std::string& fileName);
    
private:

    const float WINDOW_PADDING = 10.0f;
    
    cocos2d::Size _firstDisplaySize;
    
    cocos2d::Size _minimapSize;
    
    float _menuBarHeight;
    
    float _statusBarHeight;
    
    int _tileWidth;
    
    int _tileHeight;
    
    cocos2d::Size _workSpaceSize;
    
    cocos2d::ClippingRectangleNode* _tileRoot;
    
    std::vector<std::vector<std::string>> _tileInfo;
    
    std::vector<std::vector<cocos2d::Sprite*>> _tileImages;
    
};

#endif // __HELLOWORLD_SCENE_H__
