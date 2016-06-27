#include <fstream>
using namespace std;

#include "FileSystem.hpp"

#include "EditScene.h"
#include "GMXLayer.hpp"
#include "imgui.h"
#include "SizeProtocol.h"
#include "PaletteWindow.hpp"
#include "TriggerEditor.hpp"
#include "NewFileWindow.hpp"
#include "OpenFileWindow.hpp"
#include "SaveAsFileWindow.hpp"
#include "ImGuiGLViewImpl.h"
#include "TestScene.hpp"
using namespace cocos2d;


EditScene::EditScene()
{
}


cocos2d::Scene* EditScene::createScene()
{
    auto scene = Scene::create();
    auto layer = EditScene::create();
    scene->addChild(layer);
    return scene;
}


bool EditScene::init()
{
    if ( !ImGuiLayer::init() )
    {
        return false;
    }
    
    for(int i = 0 ; i < 256 ; ++ i) _isKeyPressed[i] = false;
    _isMousePressed = false;
    
    auto keylistener = EventListenerKeyboard::create();
    keylistener->onKeyPressed = CC_CALLBACK_2(EditScene::onKeyPressed, this);
    keylistener->onKeyReleased = CC_CALLBACK_2(EditScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
    
    auto mouselistener = EventListenerMouse::create();
    mouselistener->onMouseDown = CC_CALLBACK_1(EditScene::onMouseDown, this);
    mouselistener->onMouseUp = CC_CALLBACK_1(EditScene::onMouseUp, this);
    mouselistener->onMouseMove = CC_CALLBACK_1(EditScene::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouselistener, this);
    
    _oldWindowSize = Director::getInstance()->getVisibleSize();
    
    addImGUI([this]() {
        
        // main menu
        if ( _showNewMap ) _newFileWindow->showNewFileWindow(&_showNewMap);
        
        if ( _showOpenMap ) _openFileWindow->showOpenFileWindow(&_showOpenMap);
        
        if ( _showSaveAsFile ) _saveAsFileWindow->showSaveAsFileWindow(&_showSaveAsFile);
        
        if ( _showPalette ) _palette->showPaletteWindow(&_showPalette);
        
        if ( _showFileMenuBar ) showFileMenuBar(&_showFileMenuBar);
            
        if ( _showTrigger ) _triggerEditor->showTriggerEditor(&_showTrigger);
        
        
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File", _isFileEnable))
            {
                if (ImGui::MenuItem("New", "Ctrl+N", &_showNewMap))
                {
                    if ( _showNewMap ) _isFileEnable = false;
                }
                if (ImGui::MenuItem("Open", "Ctrl+O", &_showOpenMap))
                {
                    if ( _showOpenMap ) _isFileEnable = false;
                }
                if (ImGui::BeginMenu("Open Recent"))
                {
                    ImGui::MenuItem("fish_hat.c");
                    ImGui::MenuItem("fish_hat.inl");
                    ImGui::MenuItem("fish_hat.h");
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Save", "Ctrl+S", false, _isUndo)) { save(); }
                if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S", &_showSaveAsFile, _isUndo)) { saveAs(); }
                ImGui::Separator();
                if (ImGui::MenuItem("Quit", "Alt+F4")) {}
                
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit", _isEditEnable))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z", false, _isUndo)) { undo(); }
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, _isRedo)) { redo(); }
                
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X", false, false)) {}
                if (ImGui::MenuItem("Copy", "CTRL+C", false, false)) {}
                if (ImGui::MenuItem("Paste", "CTRL+V", false, false)) {}
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Players", _isPlayerEnable))
            {
                if (ImGui::MenuItem("Player 1")) {}
                if (ImGui::MenuItem("Player 2")) {}
                if (ImGui::MenuItem("Player 3")) {}
                if (ImGui::MenuItem("Player 4")) {}
                if (ImGui::MenuItem("Player 5")) {}
                if (ImGui::MenuItem("Player 6")) {}
                if (ImGui::MenuItem("Player 7")) {}
                if (ImGui::MenuItem("Player 8")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Setting")) {}
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Windows", _isWindowEnable))
            {
                ImGui::MenuItem("Trigger Editor", "SHIFT+T", &_showTrigger);
                ImGui::MenuItem("Palette Window", "SHIFT+P", &_showPalette);
                ImGui::MenuItem("Property Editor", "SHIFT+R", &_showProperty);
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("Contact")) {}
                if (ImGui::MenuItem("About")) {}
                ImGui::EndMenu();
            }
            
            ImGui::EndMainMenuBar();
        }
        
        // bottom menu
        ImGuiIO& io = ImGui::GetIO();
        
        ImGui::SetNextWindowPos(ImVec2(0.0f, io.DisplaySize.y - SizeProtocol::STATUSBAR_HEIGHT));
        ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, SizeProtocol::STATUSBAR_HEIGHT));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::Begin("##BottomMenuBar", NULL,
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoSavedSettings);
        
        ImGui::Columns(4, "extra_info", false);
        
        ImGui::Text("World Position (%.0f, %.0f)", _worldPosition.x, _worldPosition.y);
        
        ImGui::SameLine();
        ImGui::NextColumn();
        ImGui::Text("Player 1");
        
        ImGui::SameLine();
        ImGui::NextColumn();
        ImGui::Text("Object 372");
        
        ImGui::SameLine();
        ImGui::NextColumn();
        ImGui::Text("(%.2f FPS)", ImGui::GetIO().Framerate);
        
        ImGui::End();
        ImGui::PopStyleVar(1);
        
        // property ui
        ImGuiStyle& style = ImGui::GetStyle();
        
        ImGui::SetNextWindowPos(ImVec2(SizeProtocol::WINDOW_PADDING, SizeProtocol::MENUBAR_HEIGHT + SizeProtocol::MINIMAP_SIZE + SizeProtocol::WINDOW_PADDING * 2));
        ImGui::SetNextWindowSize(ImVec2(SizeProtocol::MINIMAP_SIZE, io.DisplaySize.y - (SizeProtocol::MENUBAR_HEIGHT + SizeProtocol::MINIMAP_SIZE + SizeProtocol::STATUSBAR_HEIGHT + SizeProtocol::WINDOW_PADDING * 3)));
        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.8200000, 0.8200000, 0.8200000, 1.0000000));
        if (ImGui::Begin(" Entities", NULL,
                         ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoBringToFrontOnFocus |
                         ImGuiWindowFlags_NoMove))
        {
            static int selected = 0;
            ImGui::BeginChild("##ObjectList", ImVec2(SizeProtocol::MINIMAP_SIZE - style.WindowPadding.x * 2, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
            for (int i = 0; i < 1; i++)
            {
                std::string label;
                label = "My Object " + std::to_string(i);
                if (ImGui::Selectable(label.c_str(), selected == i))
                    selected = i;
            }
            ImGui::EndChild();
        }
        ImGui::End();
        ImGui::PopStyleColor(1);
        
    }, "ui_init");
    
    
    addImGUI([this]{
        
        static bool isShowDemo = true;
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
        ImGui::ShowTestWindow(&isShowDemo);
        
    }, "test_window");
    
    _gmxLayerManager = GMXLayerManager::create();
    _gmxLayerManager->setPosition(Vec2(SizeProtocol::MINIMAP_SIZE + SizeProtocol::WINDOW_PADDING * 2, SizeProtocol::STATUSBAR_HEIGHT + SizeProtocol::WINDOW_PADDING));
    addChild(_gmxLayerManager);
    
    _minimapLayer = MinimapLayer::create(Size(SizeProtocol::MINIMAP_SIZE, SizeProtocol::MINIMAP_SIZE));
    _minimapLayer->setPosition(Vec2(SizeProtocol::WINDOW_PADDING, _director->getVisibleSize().height - SizeProtocol::MENUBAR_HEIGHT - SizeProtocol::MINIMAP_SIZE - SizeProtocol::WINDOW_PADDING));
    addChild(_minimapLayer);

    
    _debugNode = DrawNode::create();
    _debugNode->setPosition(Vec2(SizeProtocol::MINIMAP_SIZE + SizeProtocol::WINDOW_PADDING * 2, SizeProtocol::STATUSBAR_HEIGHT + SizeProtocol::WINDOW_PADDING));
    addChild(_debugNode);
    
    _newFileWindow = NewFileWindow::create(this);
    addChild(_newFileWindow);
    
    _openFileWindow = OpenFileWindow::create(this);
    addChild(_openFileWindow);
    
    _saveAsFileWindow = SaveAsFileWindow::create(this);
    addChild(_saveAsFileWindow);
    
    this->scheduleUpdate();
    
    return true;
}


void EditScene::update(float dt)
{
    static auto director = Director::getInstance();
    Size currSize = director->getVisibleSize();
    if ( _oldWindowSize.width != currSize.width || _oldWindowSize.height != currSize.height)
    {
        // resize
        onResize();
    }
    
    _oldWindowSize = director->getVisibleSize();
    
    GMXLayer* currLayer = _gmxLayerManager->getCurrentLayer();
    
    if ( _isKeyPressed[static_cast<int>(EventKeyboard::KeyCode::KEY_LEFT_ARROW)] == true )
    {
        if ( currLayer )
        {
            _viewSpaceParams.x = _viewSpaceParams.x - (3000.0f / currLayer->getWorldSize().width) * dt;
            _viewSpaceParams.x = clampf(_viewSpaceParams.x, 0.0f, 1.0f);
            
            onCenterView();
        }
    }
    else if ( _isKeyPressed[static_cast<int>(EventKeyboard::KeyCode::KEY_RIGHT_ARROW)] == true )
    {
        if ( currLayer )
        {
            _viewSpaceParams.x = _viewSpaceParams.x + (3000.0f / currLayer->getWorldSize().width) * dt;
            _viewSpaceParams.x = clampf(_viewSpaceParams.x, 0.0f, 1.0f);
            
            onCenterView();
        }
    }
    
    if ( _isKeyPressed[static_cast<int>(EventKeyboard::KeyCode::KEY_DOWN_ARROW)] == true )
    {
        if ( currLayer )
        {
            _viewSpaceParams.y = _viewSpaceParams.y - (3000.0f / currLayer->getWorldSize().height) * dt;
            _viewSpaceParams.y = clampf(_viewSpaceParams.y, 0.0f, 1.0f);
            
            onCenterView();
        }
    }
    else if ( _isKeyPressed[static_cast<int>(EventKeyboard::KeyCode::KEY_UP_ARROW)] == true )
    {
        if ( currLayer )
        {
            _viewSpaceParams.y = _viewSpaceParams.y + (3000.0f / currLayer->getWorldSize().height) * dt;
            _viewSpaceParams.y = clampf(_viewSpaceParams.y, 0.0f, 1.0f);
            
            onCenterView();
        }
    }
    
    if ( _gmxLayerManager->getCurrentLayer() )
    {
        _isUndo = _gmxLayerManager->getCurrentLayer()->isUndo();
        _isRedo = _gmxLayerManager->getCurrentLayer()->isRedo();
    }
}


void EditScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    _isKeyPressed[static_cast<int>(keyCode)] = true;
    
    if ( _isCtrl && keyCode == EventKeyboard::KeyCode::KEY_N )
    {
        _showNewMap = true;
    }
    if ( _isCtrl && keyCode == EventKeyboard::KeyCode::KEY_Z )
    {
        undo();
    }
    
    if ( _isCtrl && keyCode == EventKeyboard::KeyCode::KEY_Y )
    {
        redo();
    }
    
    if ( keyCode == EventKeyboard::KeyCode::KEY_CTRL ) { _isCtrl = true; }
    else if ( keyCode == EventKeyboard::KeyCode::KEY_SHIFT ) { _isShift = true; }
}


void EditScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    _isKeyPressed[static_cast<int>(keyCode)] = false;
    if ( keyCode == EventKeyboard::KeyCode::KEY_CTRL ) _isCtrl = false;
    else if ( keyCode == EventKeyboard::KeyCode::KEY_SHIFT ) _isShift = false;
}


void EditScene::onMouseDown(cocos2d::Event* event)
{
    _isMousePressed = true;
    
    auto mouseEvent = static_cast<EventMouse*>(event);
    _mousePosition.setPoint(mouseEvent->getCursorX(), mouseEvent->getCursorY());
    
    bool inImGuiWidgets = ImGui::IsPosHoveringAnyWindow(ImVec2(mouseEvent->getLocationInView().x,
                                                               _director->getVisibleSize().height - mouseEvent->getLocationInView().y));
    
    if ( inImGuiWidgets ) return ;
    
    Rect minimapRect(_minimapLayer->getPosition().x, _minimapLayer->getPosition().y, SizeProtocol::MINIMAP_SIZE, SizeProtocol::MINIMAP_SIZE);
    if ( minimapRect.containsPoint(_mousePosition) )
    {
        Size focusWindowSize = _minimapLayer->getFocusWindowSize();
        Vec2 innerPosition = _mousePosition - _minimapLayer->getPosition();
        _viewSpaceParams.setPoint((innerPosition.x - focusWindowSize.width / 2) / (SizeProtocol::MINIMAP_SIZE - focusWindowSize.width),
                                  (innerPosition.y - focusWindowSize.height / 2) / (SizeProtocol::MINIMAP_SIZE - focusWindowSize.height));
        
        _viewSpaceParams.x = clampf(_viewSpaceParams.x, 0.0, 1.0);
        _viewSpaceParams.y = clampf(_viewSpaceParams.y, 0.0, 1.0);
        
        onCenterView();
    }
    
    auto currLayer = _gmxLayerManager->getCurrentLayer();
    if ( currLayer )
    {
        if ( _palette->getPaletteType() == PaletteType::TILE )
        {
            Size clipSize = currLayer->getClippingRegion().size;
            Vec2 positionInClipRect(_mousePosition - Vec2(SizeProtocol::MINIMAP_SIZE + SizeProtocol::WINDOW_PADDING * 2, SizeProtocol::STATUSBAR_HEIGHT + SizeProtocol::WINDOW_PADDING));
            
            if ( currLayer->getClippingRegion().containsPoint(positionInClipRect) )
            {
                _worldPosition = _gmxLayerManager->getCurrentLayer()->getCenterViewPosition();
                positionInClipRect -= clipSize / 2;
                _worldPosition += positionInClipRect;
                
                auto indices = currLayer->getFocusedTileIndex(_worldPosition);
                currLayer->putTile(_palette->getSelectedItem(), indices.first, indices.second);
            }
        }
    }
}


void EditScene::onMouseMove(cocos2d::Event* event)
{
    auto mouseEvent = static_cast<EventMouse*>(event);
    _mousePosition.setPoint(mouseEvent->getCursorX(), mouseEvent->getCursorY());
    
    if ( _isMousePressed )
    {
        Rect minimapRect(_minimapLayer->getPosition().x, _minimapLayer->getPosition().y, SizeProtocol::MINIMAP_SIZE, SizeProtocol::MINIMAP_SIZE);
        if ( minimapRect.containsPoint(_mousePosition) )
        {
            Size focusWindowSize = _minimapLayer->getFocusWindowSize();
            Vec2 innerPosition = _mousePosition - _minimapLayer->getPosition();
            _viewSpaceParams.setPoint((innerPosition.x - focusWindowSize.width / 2) / (SizeProtocol::MINIMAP_SIZE - focusWindowSize.width),
                                      (innerPosition.y - focusWindowSize.height / 2) / (SizeProtocol::MINIMAP_SIZE - focusWindowSize.height));
            
            _viewSpaceParams.x = clampf(_viewSpaceParams.x, 0.0, 1.0);
            _viewSpaceParams.y = clampf(_viewSpaceParams.y, 0.0, 1.0);
        }
    }
    
    onCenterView();
}


void EditScene::onMouseUp(cocos2d::Event* event)
{
    _isMousePressed = false;
}


void EditScene::onCenterView()
{
    _gmxLayerManager->onCenterView(_viewSpaceParams.x, _viewSpaceParams.y);
    
    if ( _minimapLayer ) _minimapLayer->onCenterView(_viewSpaceParams);
    
    auto currLayer = _gmxLayerManager->getCurrentLayer();
    if ( currLayer )
    {
        if ( _palette->getPaletteType() == PaletteType::TILE )
        {
            Size clipSize = currLayer->getClippingRegion().size;
            Vec2 positionInClipRect(_mousePosition - Vec2(SizeProtocol::MINIMAP_SIZE + SizeProtocol::SizeProtocol::WINDOW_PADDING * 2, SizeProtocol::STATUSBAR_HEIGHT + SizeProtocol::WINDOW_PADDING));
            positionInClipRect.x = clampf(positionInClipRect.x, 0.0f, clipSize.width);
            positionInClipRect.y = clampf(positionInClipRect.y, 0.0f, clipSize.height);
            
            _worldPosition = _gmxLayerManager->getCurrentLayer()->getCenterViewPosition();
            positionInClipRect -= clipSize / 2;
            _worldPosition += positionInClipRect;
            
            auto indices = currLayer->getFocusedTileIndex(_worldPosition);
            currLayer->drawSelectRegion(indices.first, indices.second);
            currLayer->setSelectTileImage(_palette->getSelectedItem());
        }
        else if ( _palette->getPaletteType() == PaletteType::ENTITY )
        {
            currLayer->disableSelectRegion();
        }
        else if ( _palette->getPaletteType() == PaletteType::ITEM )
        {
            currLayer->disableSelectRegion();
        }
        else if ( _palette->getPaletteType() == PaletteType::DOODAD )
        {
            currLayer->disableSelectRegion();
        }
    }
}


void EditScene::createNewFile(GMXFile* file)
{
    auto newLayer = GMXLayer::create();
    newLayer->openFile(file);
    
    _palette = PaletteWindow::create(this);
    newLayer->addChild(_palette);
    
    _triggerEditor = TriggerEditor::create(this);
    newLayer->addChild(_triggerEditor);
    
    _gmxLayerManager->addChild(newLayer);
    _minimapLayer->setGMXLayer(newLayer);
    
    _isEditEnable = true;
    _isPlayerEnable = true;
    _isWindowEnable = true;
    _showFileMenuBar = true;
    _showPalette = true;
    _isFileEnable = true;
}


void EditScene::onResize()
{
    SizeProtocol::MINIMAP_SIZE = _director->getVisibleSize().width * 0.15f;
    _gmxLayerManager->setPosition(Vec2(SizeProtocol::MINIMAP_SIZE + SizeProtocol::WINDOW_PADDING * 2, SizeProtocol::STATUSBAR_HEIGHT + SizeProtocol::WINDOW_PADDING));
    
    if ( _gmxLayerManager->getCurrentLayer() )
    {
        _gmxLayerManager->onResize();
    }
    _minimapLayer->onResize();
}


void EditScene::showFileMenuBar(bool* opened)
{
    ImGui::SetNextWindowPos(ImVec2(SizeProtocol::MINIMAP_SIZE + SizeProtocol::WINDOW_PADDING * 2, SizeProtocol::MENUBAR_HEIGHT + SizeProtocol::WINDOW_PADDING));
    ImGui::SetNextWindowSize(ImVec2(_oldWindowSize.width - SizeProtocol::MINIMAP_SIZE - SizeProtocol::WINDOW_PADDING * 3, SizeProtocol::FILE_MENUBAR_HEIGHT));
    
    std::string fileName = _gmxLayerManager->getCurrentLayer()->getFileName();
    if ( _gmxLayerManager->getCurrentLayer()->isChanged() )
    {
        fileName += " *";
    }
    
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.8200000, 0.8200000, 0.8200000, 1.0000000));
    ImGui::Begin(fileName.c_str(), &_showFileMenuBar, ImVec2(0,0), 0.0f,
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoBringToFrontOnFocus);
    
    ImGui::End();
    ImGui::PopStyleColor(1);
    
    if ( *opened == false )
    {
        // closed
        closeCurrentLayer();
    }
}



void EditScene::redo()
{
    if ( _gmxLayerManager->getCurrentLayer() && _isRedo )
    {
        _gmxLayerManager->getCurrentLayer()->redo();
    }
}


void EditScene::undo()
{
    if ( _gmxLayerManager->getCurrentLayer() && _isUndo )
    {
        _gmxLayerManager->getCurrentLayer()->undo();
    }
}


void EditScene::save()
{
    auto currLayer = _gmxLayerManager->getCurrentLayer();
    if ( currLayer )
    {
        if ( currLayer->isFirstFile() )
        {
            // show save
            _showSaveAsFile = true;
            
            _isFileEnable = false;
            _isEditEnable = false;
            _isWindowEnable = false;
            _isPlayerEnable = false;
        }
        else
        {
            boost::filesystem::remove(currLayer->getFilePath());
            _gmxLayerManager->saveGMXFile(currLayer->getFilePath());
            log("[%s] Saved.", currLayer->getFilePath().c_str());
        }
    }
}


void EditScene::save(const std::string& path)
{
    if (!boost::filesystem::exists(path))
    {
        _gmxLayerManager->saveGMXFile(path);
        _gmxLayerManager->getCurrentLayer()->disableFirstFile();
        log("[%s] Saved.", path.c_str());
    }
    else
    {
        log("[%s] is already exists.", path.c_str());
    }

}


void EditScene::saveAs()
{
    
}


void EditScene::open(const std::string& path)
{
    if ( _gmxLayerManager->getCurrentLayer() )
    {
        closeCurrentLayer();
    }
    
    GMXFile* file = new GMXFile();
    
    if ( _gmxLayerManager->loadGMXFile(file, path) )
    {
        auto newLayer = GMXLayer::create();
        newLayer->setFilePath(path);
        newLayer->disableFirstFile();
        newLayer->openFile(file);
        
        _palette = PaletteWindow::create(this);
        newLayer->addChild(_palette);
        
        _gmxLayerManager->addChild(newLayer);
        _minimapLayer->setGMXLayer(newLayer);
        
        
        _isEditEnable = true;
        _isPlayerEnable = true;
        _isWindowEnable = true;
        _showFileMenuBar = true;
        _showPalette = true;
    }
    else
    {
        delete file;
        file = nullptr;
        
        log("open file fail!");
    }
}


void EditScene::closeCurrentLayer()
{
    _showPalette = false;
    _showTrigger = false;
    _isRedo = false;
    _isWindowEnable = false;
    _isPlayerEnable = false;
    _isEditEnable = false;
    _isUndo = false;
    Dispatch.removeNode(MessageNodeType::GMX_LAYER);
    _viewSpaceParams.setZero();
    _worldPosition = Vec2::ZERO;
    _minimapLayer->disableFocusWindow();
    _gmxLayerManager->closeLayer();

}










