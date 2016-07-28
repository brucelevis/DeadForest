//
//  PlayGameLayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 28..
//
//

#pragma once

#include "cocos2d.h"

#include "MainMenu3.hpp"
#include "GameWorld.hpp"
#include "DummyScene.hpp"

#include "imgui.h"
#include "imgui_internal.h"

namespace realtrick
{
    namespace editor
    {
        
        class EditScene;
        
        class PlayGameLayer : public cocos2d::Node
        {
            
        public:
            
            explicit PlayGameLayer(EditScene* layer) : _imguiLayer(layer)
            {}
            virtual ~PlayGameLayer() = default;
            
            static PlayGameLayer* create(EditScene* layer)
            {
                auto ret = new (std::nothrow) PlayGameLayer(layer);
                if ( ret && ret->init() )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            virtual bool init() override
            {
                if ( !cocos2d::Node::init() )
                    return false;
                
                
                return true;
            }
            
            void showLayer(bool& opened)
            {
                ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiSetCond_Once);
                ImGui::SetNextWindowSize(ImVec2(1136, 640), ImGuiSetCond_Once);
                ImGui::Begin("simulator", &opened, ImVec2(0,0), 0.0f,
                             ImGuiWindowFlags_ShowBorders |
                             ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoCollapse);
                
                auto canvasSize = cocos2d::Size(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
                ImGui::InvisibleButton("##dummy", ImVec2(canvasSize.width, canvasSize.height));
                setPosition(ImGui::GetWindowPos().x, cocos2d::Director::getInstance()->getVisibleSize().height - ImGui::GetWindowPos().y - 640);
                ImGui::End();
                
                if ( !opened )
                {
                    _gameLayer->removeFromParentAndCleanup(true);
                    _imguiLayer->stopGame();
                }
                
            }
            
            void playGame()
            {
                _gameLayer = realtrick::client::DummyScene::create();
                addChild(_gameLayer);
            }
    
            
        private:
            
            EditScene* _imguiLayer;
            cocos2d::Node* _gameLayer;
            
        };
        
    }
}









