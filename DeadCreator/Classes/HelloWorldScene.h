#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "GMXLayerManager.hpp"
#include "PaletteWindow.hpp"

class GMXFile;
class GMXLayer;
class GMXLayerManager;
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
    
    void saveGMXLayer(GMXFile* file, const std::string fileName);
    
    void loadGMXLayer(GMXFile* file, const std::string fileName);
    
    void showNewMapWindow(bool* opened);
    
    void showFileMenuBar(bool* opened);
    
private:

    cocos2d::Size _oldWindowSize;
    
    GMXLayerManager* _gmxLayerManager;
    
    MinimapLayer* _minimapLayer;
    
    cocos2d::Vec2 _centerPosition;
    
    cocos2d::Vec2 _viewSpaceParams;
    
    bool _isKeyPressed[256];
    
    cocos2d::Vec2 _mousePosition;
    
    PaletteWindow* _palette;
    
    bool _isMousePressed;
    
    bool _isMenuSelected = false;
    bool _showNewMap = false;
    bool _showTrigger = false;
    bool _showProperty = false;
    bool _showPalette = false;
    bool _showFileMenuBar = false;
    bool _isSaveEnable = false;
    bool _isEditEnable = false;
    bool _isPlayerEnable = false;
    bool _isWindowEnable = false;
};

#endif // __HELLOWORLD_SCENE_H__
