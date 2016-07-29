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

#include "imgui.h"
#include "imgui_internal.h"


void PlayGameLayer::showLayer(bool& opened)
{
    static bool isFPSOn = true;
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
    
    setPosition(ImGui::GetWindowPos().x, cocos2d::Director::getInstance()->getVisibleSize().height - ImGui::GetWindowPos().y - 640 - 25);
    
    auto origin = ImGui::GetCursorScreenPos();
    auto canvasSize = cocos2d::Size(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    ImGui::InvisibleButton("##dummy", ImVec2(canvasSize.width, canvasSize.height));
    
    // game layer
    if ( _isGameStarted )
    {
        auto drawList = ImGui::GetWindowDrawList();
        
        ImGui::SetCursorScreenPos(origin);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00, 1.00, 1.00, 1.00));
        if ( isFPSOn )
        {
            ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
        }
        
        drawList->AddCircle(ImVec2(origin.x + 50, origin.y + 50), 30, ImColor(ImVec4(1,1,1,1)));
        
        ImGui::PopStyleColor();
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
            ImGui::Checkbox("fps", &isFPSOn);
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
}

void PlayGameLayer::closeLayer()
{
    _gameLayer->removeFromParentAndCleanup(true);
    _imguiLayer->stopGame();
}









