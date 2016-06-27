//
//  GMXLayer2.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 28..
//
//

#include "GMXLayer2.hpp"
#include "TerrainBase.hpp"
using namespace cocos2d;

GMXLayer2* GMXLayer2::create(const cocos2d::Size& size)
{
    auto ret = new (std::nothrow) GMXLayer2();
    if ( ret && ret->init(size) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool GMXLayer2::init(const cocos2d::Size& size)
{
    if ( !ImGuiLayer::init() )
        return false;
    
    _layerSize = size;
    
    addImGUI([this] {
        
        //auto visibleSize = Director::getInstance()->getVisibleSize();
        
        if ( _isShowWindow ) showWindow();
        
    }, "##GMXLayer");
    
    
    return true;
}


void GMXLayer2::showWindow()
{
    auto windowSize = Size(500, 20);
    
    ImGui::SetNextWindowPos(ImVec2(_position.x, _position.y), ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2(windowSize.width, windowSize.height));
    
    //        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.8200000, 0.8200000, 0.8200000, 1.0000000));
    ImGui::Begin("test layer", &_isShowWindow, ImVec2(0,0), 0.0f,
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoBringToFrontOnFocus);
    
    ImGui::End();
    //        ImGui::PopStyleColor(1);
}


void GMXLayer2::update(float dt)
{
}


void GMXLayer2::setTerrain(int x, int y, TerrainBase* terrain)
{
    _tileImages[y][x]->setTexture(terrain->getNumber() + ".png");
    MutableUiBase::setTerrain(x, y, terrain);
}

