//
//  NavigatorLayer.cpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 28..
//
//

#include "NavigatorLayer.hpp"
#include "EditScene2.hpp"
#include "GMXLayer2.hpp"
using namespace cocos2d;

NavigatorLayer::NavigatorLayer(EditScene2& imguiLayer) :
_imguiLayer(imguiLayer),
_worldDebugNode(nullptr),
_localDebugNode(nullptr),
_clipNode(nullptr),
_visibleSize(Director::getInstance()->getVisibleSize()),
_layerSize(Size(200,200)),
_layerPosition(Vec2(50, 50)),
_centerViewParam(Vec2::ZERO),
_centerViewPosition(Vec2(_layerSize / 2)),
_rootNode(nullptr)
{
}


NavigatorLayer::~NavigatorLayer()
{
}


NavigatorLayer* NavigatorLayer::create(EditScene2& imguiLayer)
{
    auto ret = new (std::nothrow) NavigatorLayer(imguiLayer);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool NavigatorLayer::init()
{
    if ( !Node::init() )
        return false;
    
    _clipNode = ClippingRectangleNode::create();
    addChild(_clipNode);
    
    _rootNode = Node::create();
    _clipNode->addChild(_rootNode , 10);
    
    _worldDebugNode = DrawNode::create();
    _rootNode->addChild(_worldDebugNode);
    
    _localDebugNode = DrawNode::create();
    _clipNode->addChild(_localDebugNode);
    
    auto spr = Sprite::create("1_1_1234.png");
    _rootNode->addChild(spr);
    
    _imguiLayer.addImGUI([this] {
        
        if ( static_cast<GMXLayer2*>(getParent())->isShowNavigator() ) showLayer(&static_cast<GMXLayer2*>(getParent())->isShowNavigator());
        
    }, "##NavigatorLayer");
    
    
    return true;
}


void NavigatorLayer::showLayer(bool* opened)
{
    ImGui::SetNextWindowPos(ImVec2(_layerPosition.x, _layerPosition.y), ImGuiSetCond_Appearing);
    ImGui::SetNextWindowSize(ImVec2(_layerSize.width, _layerSize.height), ImGuiSetCond_Appearing);
    
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.8200000, 0.8200000, 0.8200000, 1.0000000));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
    ImGui::Begin("navigator", opened, ImVec2(0,0), 0.0f,
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_ShowBorders |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoBringToFrontOnFocus);
    
    _layerPosition.setPoint(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
    _layerSize.setSize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
    
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    
    auto parentPos = getParent()->getPosition();
    _visibleSize = Director::getInstance()->getVisibleSize();
    setPosition(_layerPosition.x - parentPos.x, _visibleSize.height - _layerPosition.y - _layerSize.height - parentPos.y);
    
    _clipNode->setClippingRegion(Rect(0, 0, _layerSize.width, _layerSize.height));
    
    _localDebugNode->clear();
    _localDebugNode->drawDot(Vec2::ZERO, 5.0f, Color4F::YELLOW);
    
    log("clipNodePosition : %.0f, %.0f", _clipNode->getPosition().x, _clipNode->getPosition().y);
    log("layer: %.0f, %.0f", _layerPosition.x, _layerPosition.y);
    log("pos: %.0f, %.0f", getPosition().x, getPosition().y);
    
    setVisible(true);
}





