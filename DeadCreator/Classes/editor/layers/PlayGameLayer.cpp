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
    
    setPosition(ImGui::GetWindowPos().x, Director::getInstance()->getVisibleSize().height - ImGui::GetWindowPos().y - 640 - 25);
    
    auto origin = ImGui::GetCursorScreenPos();
    auto canvasSize = cocos2d::Size(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    ImGui::InvisibleButton("##dummy", ImVec2(canvasSize.width, canvasSize.height));
    
    // game layer
    if ( _isGameStarted )
    {
        auto drawList = ImGui::GetWindowDrawList();
        auto game = _gameLayer->getGameManager();
        ImGui::SetCursorScreenPos(origin);
        if ( isFPSOn )
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00, 1.00, 1.00, 1.00));
            ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
            ImGui::PopStyleColor();
        }
    
        ImVec2 world(1560, 2000);
        ImVec2 local = ImVec2(world.x - game->getPlayerPtr()->getWorldPosition().x, world.y - game->getPlayerPtr()->getWorldPosition().y);
        
        drawList->AddCircle(ImVec2(568 + origin.x + local.x, 320 + origin.y - local.y), 3.0f, ImColor(ImVec4(1,1,1,1)));
        
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









