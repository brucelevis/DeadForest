//
//  HistoryLayer.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 3..
//
//

#pragma once

#include "cocos2d.h"
#include "GMXLayer.hpp"
#include "CommandQueue.hpp"
#include "CommandBase.hpp"
#include "StringHelper.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class GMXLayer;
        
        class HistoryLayer : public cocos2d::Node
        {
            
        public:
            
            explicit HistoryLayer(GMXLayer& gmxLayer) :
            _gmxLayer(gmxLayer),
            _layerSize(cocos2d::Size(200, 300)),
            _layerPosition(cocos2d::Vec2(120, 370)),
            _boundingBoxPadding(cocos2d::Rect::ZERO)
            {}
            
            virtual ~HistoryLayer() = default;
            static HistoryLayer* create(GMXLayer& gmxLayer)
            {
                auto ret = new (std::nothrow) HistoryLayer(gmxLayer);
                if ( ret && ret->init() )
                {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }
            
            void showLayer(bool& opened)
            {
                ImGuiContext& g = *GImGui;
                float height = g.FontBaseSize + g.Style.FramePadding.y * 2.0f;
                
                ImGui::SetNextWindowPos(ImVec2(_layerPosition.x, _layerPosition.y), ImGuiSetCond_Once);
                ImGui::SetNextWindowSize(ImVec2(_layerSize.width, _layerSize.height), ImGuiSetCond_Once);
                
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
                ImGui::Begin("history", &opened, ImVec2(0,0), 0.9f,
                             ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoScrollWithMouse |
                             ImGuiWindowFlags_ShowBorders);
                
                _layerPosition.setPoint(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
                _layerSize.setSize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
                
                static cocos2d::Vec2 mousePosInCocos2dMatrix;
                mousePosInCocos2dMatrix = cocos2d::Vec2(ImGui::GetIO().MousePos.x, ImGui::GetIO().DisplaySize.y - ImGui::GetIO().MousePos.y);
                if ( ImGui::IsMouseHoveringWindow() && ImGui::GetIO().MouseClicked[0] )
                {
                    cocos2d::Rect boundingBox(_layerPosition.x, ImGui::GetIO().DisplaySize.y - _layerSize.height - _layerPosition.y, _layerSize.width, _layerSize.height);
                    if ( boundingBox.containsPoint(mousePosInCocos2dMatrix) )
                    {
                        GMXLayer::enableTitleClicked();
                    }
                }
                
                _boundingBoxPadding.setRect(_layerPosition.x + g.Style.WindowPadding.x,
                                            _layerPosition.y + g.Style.WindowPadding.y + height,
                                            _layerSize.width - g.Style.WindowPadding.x * 2.0f,
                                            _layerSize.height - g.Style.WindowPadding.y * 2.0f - height);
                
                ImGui::BeginChild("##child", ImVec2(0, _layerSize.height - height - g.Style.FramePadding.y * 2.0f), true);
                {
                    static cocos2d::Vec2 mousePosInCocos2dMatrix;
                    mousePosInCocos2dMatrix = cocos2d::Vec2(ImGui::GetIO().MousePos.x,
                                                            ImGui::GetIO().DisplaySize.y - ImGui::GetIO().MousePos.y);
                    if ( ImGui::IsMouseHoveringWindow() && ImGui::GetIO().MouseClicked[0] )
                    {
                        cocos2d::Rect boundingBox(_layerPosition.x, ImGui::GetIO().DisplaySize.y - _layerSize.height - _layerPosition.y, _layerSize.width, _layerSize.height);
                        if ( boundingBox.containsPoint(mousePosInCocos2dMatrix) )
                        {
                            GMXLayer::enableTitleClicked();
                        }
                    }
                }
                
                int currIndex = _commandQueue.getIndex();
                for(int i = 0 ; i < _commandQueue.size() ; ++ i)
                {
                    bool isPrev = (currIndex > i);
                    bool isCurr = (currIndex == i);
                    bool isNext = (currIndex < i);
                    bool isSelected = false;
                    
                    if ( isPrev )
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0000, 0.0000, 0.0000, 1.0000));
                    }
                    else if ( isCurr )
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0000, 1.0000, 1.0000, 1.0000));
                        isSelected = true;
                    }
                    else if ( isNext )
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0000, 0.0000, 0.0000, 0.5000));
                    }
                    
                    std::string commandName = _commandQueue[i]->getCommandName()  + "[" + _to_string(i) + "]";
                    if ( ImGui::Selectable(commandName.c_str(), isSelected) )
                    {
                        _commandQueue.setStateToIndex(i);
                        _gmxLayer.updateCollisionRegion();
                    }
                    
                    ImGui::PopStyleColor();
                    ImGui::Separator();
                }
                
                if ( _isSetScrollBottom )
                {
                    ImGui::SetScrollHere(1.0f);
                    _isSetScrollBottom = false;
                }
                
                ImGui::EndChild();
                
                ImGui::End();
                ImGui::PopStyleVar();
            }
            
            void pushCommand(CommandBase* command) { _commandQueue.pushCommand(command); _isSetScrollBottom = true; }
            
            void redo() { if ( isRedo() ) _commandQueue.redo(); }
            void undo() { if ( isUndo() ) _commandQueue.undo(); }
            
            bool isRedo() const { return _commandQueue.isRedo(); }
            bool isUndo() const { return _commandQueue.isUndo(); }
            
            int getIndex() const { return _commandQueue.getIndex(); }
            int size() const { return _commandQueue.size(); }
            
        private:
            
            GMXLayer& _gmxLayer;
            
            cocos2d::Size _layerSize;
            cocos2d::Vec2 _layerPosition;
            cocos2d::Rect _boundingBoxPadding;
            
            CommandQueue _commandQueue;
            bool _isSetScrollBottom = false;
            
        };
        
    }
}









