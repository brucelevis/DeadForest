//
//  HistoryLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 3..
//
//

#include "HistoryLayer.hpp"
#include "GMXLayer2.hpp"
#include "CommandBase.hpp"
using namespace cocos2d;

HistoryLayer::HistoryLayer(GMXLayer2& gmxLayer) :
_gmxLayer(gmxLayer),
_layerSize(Size(200, 300)),
_layerPosition(Vec2(120, 70)),
_boundingBoxPadding(Rect::ZERO)
{
}


HistoryLayer::~HistoryLayer()
{
}


HistoryLayer* HistoryLayer::create(GMXLayer2& gmxLayer)
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


bool HistoryLayer::init()
{
    if ( !Node::init() )
        return false;
    
    return true;
}


void HistoryLayer::showLayer(bool* opened)
{
    ImGuiState& g = *GImGui;
    float height = g.FontBaseSize + g.Style.FramePadding.y * 2.0f;
    
    if ( _layerPosition.x < WINDOW_PADDING )
    {
        _layerPosition.x = WINDOW_PADDING;
    }
    
    if ( _layerPosition.y < height + WINDOW_PADDING )
    {
        _layerPosition.y = height + WINDOW_PADDING;
    }
    
    if ( _layerPosition.x + _layerSize.width > g.IO.DisplaySize.x - WINDOW_PADDING )
    {
        _layerPosition.x = g.IO.DisplaySize.x - _layerSize.width - WINDOW_PADDING;
    }
    
    if ( _layerPosition.y + _layerSize.height > g.IO.DisplaySize.y - WINDOW_PADDING - STATUSBAR_HEIGHT )
    {
        _layerPosition.y = g.IO.DisplaySize.y - _layerSize.height - WINDOW_PADDING - STATUSBAR_HEIGHT;
    }

    ImGui::SetNextWindowPos(ImVec2(_layerPosition.x, _layerPosition.y), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(_layerSize.width, _layerSize.height), ImGuiSetCond_Once);
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
    ImGui::Begin("history", opened, ImVec2(0,0), 0.9f,
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoScrollWithMouse |
                 ImGuiWindowFlags_ShowBorders);
    
    _layerPosition.setPoint(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
    _layerSize.setSize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
    
    _boundingBoxPadding.setRect(_layerPosition.x + g.Style.WindowPadding.x,
                                _layerPosition.y + g.Style.WindowPadding.y + height,
                                _layerSize.width - g.Style.WindowPadding.x * 2.0f,
                                _layerSize.height - g.Style.WindowPadding.y * 2.0f - height);
    
    ImGui::BeginChild("##child", ImVec2(0, _layerSize.height - height - g.Style.FramePadding.y * 2.0f), true);
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
        
        std::string commandName = _commandQueue[i]->getCommandName()  + "[" + std::to_string(i) + "]";
        if ( ImGui::Selectable(commandName.c_str(), isSelected) )
        {
            _commandQueue.setStateToIndex(i);
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









