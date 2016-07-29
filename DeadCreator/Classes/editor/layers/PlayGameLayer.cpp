//
//  PlayGameLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 28..
//
//

#include "MainMenu3.hpp"
#include "GameWorld.hpp"
#include "GameManager.hpp"
#include "DummyScene.hpp"
#include "EditScene.hpp"
using namespace realtrick::editor;
using namespace realtrick::client;
using namespace cocos2d;

#include "imgui.h"
#include "imgui_internal.h"


void PlayGameLayer::showLayer(bool& opened)
{
    static bool isStatusOn = true;
    static bool isCellSpaceOn = false;
    static bool isIntersectionViewOn = false;
    static bool isNavGraphOn = false;
    static bool isLocationViewOn = false;
    
    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(1136, 665), ImGuiSetCond_Once);
    
    ImGui::Begin("simulator", &opened, ImVec2(0,0), 0.0f,
                 ImGuiWindowFlags_ShowBorders |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoBringToFrontOnFocus);
    
    setPosition(ImGui::GetWindowPos().x, Director::getInstance()->getVisibleSize().height - ImGui::GetWindowPos().y - 640 - 25);
    
    auto origin = ImGui::GetCursorScreenPos();
    auto canvasSize = cocos2d::Size(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    ImGui::InvisibleButton("##dummy", ImVec2(canvasSize.width, canvasSize.height));
    
    // game layer
    if ( _isGameStarted )
    {
        ImGui::SetCursorScreenPos(origin);
        
        auto game = _gameLayer->getGameManager();
        
        _debugNode->clear();
        _debugNode->setScale(game->getZoomScale());
        
        if ( isStatusOn )
        {
            static auto renderer = Director::getInstance()->getRenderer();
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00, 1.00, 1.00, 1.00));
            ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
            ImGui::Text("Vertices: %d", static_cast<int>(renderer->getDrawnVertices()));
            ImGui::Text("Batches: %d", static_cast<int>(renderer->getDrawnBatches()));
            ImGui::PopStyleColor();
        }
        
        if ( isNavGraphOn )
        {
            Mat3 mat;
            mat.translate(game->getPlayerPtr()->getWorldPosition());
            mat.inverse();
            
            Vec2 local = mat.getTransformedVector(Vec2(1560, 2000));
            
           
            _debugNode->drawDot(local, 3.0f, Color4F::WHITE);
        }
        
    }
    ImGui::End();
    
    if ( !opened ) closeLayer();
    
    // setting layer
    if ( _isGameStarted )
    {
        ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiSetCond_Once);
        ImGui::Begin("setting", NULL, ImGuiWindowFlags_ShowBorders);
        
        if (ImGui::TreeNode("debug"))
        {
            ImGui::Checkbox("status", &isStatusOn);
            ImGui::Checkbox("cell", &isCellSpaceOn);
            ImGui::Checkbox("intersection", &isIntersectionViewOn);
            ImGui::Checkbox("graph", &isNavGraphOn);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("trigger"))
        {
            ImGui::Checkbox("location", &isLocationViewOn);
            ImGui::TreePop();
        }
        
        ImGui::End();
    }
}

void PlayGameLayer::playGame()
{
    _gameLayer = realtrick::client::DummyScene::create(this);
    addChild(_gameLayer);
    
    _clipNode = ClippingRectangleNode::create(cocos2d::Rect(0, 0, GAME_SCREEN_HEIGHT, GAME_SCREEN_HEIGHT));
    addChild(_clipNode);
    
    _debugNode = DrawNode::create();
    _debugNode->setPosition(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2);
    _clipNode->addChild(_debugNode);
}

void PlayGameLayer::closeLayer()
{
    _gameLayer->removeFromParentAndCleanup(true);
    _clipNode->removeFromParentAndCleanup(true);
    
    _imguiLayer->stopGame();
}









