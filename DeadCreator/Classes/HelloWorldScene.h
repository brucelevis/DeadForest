#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "GMXLayerManager.hpp"
#include "ImGuiLayer.h"

class GMXFile;
class GMXLayer;
class GMXLayerManager;
class MinimapLayer;
class PaletteWindow;
class TriggerEditor;
class OpenFileWindow;

class HelloWorld : public ImGuiLayer
{
    
public:
    
    HelloWorld();
    
    virtual ~HelloWorld() = default;
    
    virtual bool init() override;
    
    virtual void update(float dt) override;
    
    CREATE_FUNC(HelloWorld);
    
    static cocos2d::Scene* createScene();
    
private:
    
    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    
    void onMouseDown(cocos2d::Event* event);
    
    void onMouseMove(cocos2d::Event* event);
    
    void onMouseUp(cocos2d::Event* event);
    
    void onCenterView();
    
    void onResize();
    
    void showNewMapWindow(bool* opened);
    
    void showFileMenuBar(bool* opened);
    
    void showOpenWindow(bool* opened);
    
    //void showDirectories();
    
    void redo();
    
    void undo();
    
    void save();
    
    void saveAs() {}
    
    void open();
    
private:

    cocos2d::Size _oldWindowSize;
    
    GMXLayerManager* _gmxLayerManager;
    
    MinimapLayer* _minimapLayer;
    
    cocos2d::Vec2 _centerPosition;
    
    cocos2d::Vec2 _viewSpaceParams;
    
    bool _isKeyPressed[256];
    
    cocos2d::Vec2 _mousePosition;
    
    cocos2d::Vec2 _worldPosition;
    
    PaletteWindow* _palette;
    
    TriggerEditor* _triggerEditor;
    
    OpenFileWindow* _openFileWindow;
    
    cocos2d::DrawNode* _debugNode;
    
    bool _isMousePressed = false;
    
    bool _showNewMap = false;
    
    bool _showOpenMap = false;
    
    bool _showTrigger = false;
    
    bool _showProperty = false;
    
    bool _showPalette = false;
    
    bool _showFileMenuBar = false;
    
    bool _isFileEnable = true;
    
    bool _isEditEnable = false;
    
    bool _isPlayerEnable = false;
    
    bool _isWindowEnable = false;
    
    bool _isRedo = false;
    
    bool _isUndo = false;
    
    bool _isCtrl = false;
    
    bool _isShift = false;
};

#endif // __HELLOWORLD_SCENE_H__
