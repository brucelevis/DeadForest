#pragma once

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
class NewFileWindow;
class SaveAsFileWindow;

class EditScene : public ImGuiLayer
{
    
public:
    
    EditScene();
    
    virtual ~EditScene() = default;
    
    virtual bool init() override;
    
    virtual void update(float dt) override;
    
    CREATE_FUNC(EditScene);
    
    static cocos2d::Scene* createScene();
    
    void setEnableFileMenu(bool enable) { _isFileEnable = enable; }
    
    void setEnableEditMenu(bool enable) { _isEditEnable = enable; }
    
    void setEnableWindowMenu(bool enable) { _isWindowEnable = enable; }
    
    void setEnablePlayerMenu(bool enable) { _isPlayerEnable = enable; }
    
    void createNewFile(GMXFile* file);
    
    void open(const std::string& path);
    
    void save();
    
    void save(const std::string& path);
    
    void saveAs();
    
    void closeCurrentLayer();
    
private:
    
    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
    
    void onMouseDown(cocos2d::Event* event);
    
    void onMouseMove(cocos2d::Event* event);
    
    void onMouseUp(cocos2d::Event* event);
    
    void onCenterView();
    
    void onResize();
    
    void showFileMenuBar(bool* opened);
    
    void redo();
    
    void undo();
    
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
    
    NewFileWindow* _newFileWindow;
    
    OpenFileWindow* _openFileWindow;
    
    SaveAsFileWindow* _saveAsFileWindow;
    
    cocos2d::DrawNode* _debugNode;
    
    bool _isMousePressed = false;
    
    bool _showNewMap = false;
    
    bool _showOpenMap = false;
    
    bool _showSaveAsFile = false;
    
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

