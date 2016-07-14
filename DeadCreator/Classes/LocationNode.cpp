//
//  LocationNode.cpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 14..
//
//

#include "LocationNode.hpp"
#include "GMXLayer2.hpp"
using namespace cocos2d;
using namespace realtrick;


LocationNode::LocationNode(GMXLayer2& layer) :
_gmxLayer(layer)
{
}


LocationNode::~LocationNode()
{
}


LocationNode* LocationNode::create(GMXLayer2& layer)
{
    auto ret = new (std::nothrow) LocationNode(layer);
    if ( ret && ret->init() )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}


bool LocationNode::init()
{
    if ( !Node::init() )
        return false;
    
    _aabbNode = DrawNode::create();
    addChild(_aabbNode);
    
    setSelected(false);
    setLocationSize(3, 10);
    
    return true;
}


void LocationNode::setSelected(bool selected)
{
    _isSelected = selected;
}


void LocationNode::updateRects()
{
    //     0    1    2
    //    ___ _____ ___
    //   |  _|_____|_  |
    //   | | |     | | |
    //    --- ----- ---
    //   | | |     | | |
    // 3 | | |  4  | | | 5
    //   | | |     | | |
    //    --- ----- ---
    //   | |_|_____|_| |   _
    //   |p' |     |   |   | (dummySpace)
    //    --- ----- ---    -
    //     6    7    8
    //               |-|
    //             (dummySpace)
    
    auto file = _gmxLayer.getFile();
    Size rectSize(file.tileWidth / 4 * _sizeX, file.tileHeight / 4 * _sizeY);
    
    Vec2 p = this->getPosition();
    
    _rects[0].setRect(p.x - dummySpace, p.y + rectSize.height - dummySpace, dummySpace * 2, dummySpace * 2);
    _rects[1].setRect(p.x + dummySpace, p.y + rectSize.height - dummySpace, rectSize.width - dummySpace * 2, dummySpace * 2);
    _rects[2].setRect(p.x + rectSize.width - dummySpace, p.y + rectSize.height - dummySpace, dummySpace * 2, dummySpace * 2);
    
    _rects[3].setRect(p.x - dummySpace, p.y + dummySpace, dummySpace * 2, rectSize.height - dummySpace * 2);
    _rects[4].setRect(p.x + dummySpace, p.y + dummySpace, rectSize.width - dummySpace * 2, rectSize.height - dummySpace * 2);
    _rects[5].setRect(p.x + rectSize.width - dummySpace, p.y + dummySpace, dummySpace * 2, rectSize.height - dummySpace * 2);
    
    _rects[6].setRect(p.x - dummySpace, p.y - dummySpace, dummySpace * 2, dummySpace * 2);
    _rects[7].setRect(p.x + dummySpace, p.y - dummySpace, rectSize.width - dummySpace * 2, dummySpace * 2);
    _rects[8].setRect(p.x + rectSize.width - dummySpace, p.y - dummySpace, dummySpace * 2, dummySpace * 2);

    _aabb.setRect(p.x, p.y, rectSize.width, rectSize.height);
}


void LocationNode::update(const Vec2& mouseWorldPosition)
{
    if ( !_isSelected )
    {
        if ( _aabb.containsPoint(mouseWorldPosition) )
        {
            if ( ImGui::GetIO().MouseClicked[0] )
            {
                _isSelected = true;
            }
            else // hovered
            {
                for(int i = 0 ; i < 9 ; ++ i)
                {
                    _colors[i] = Color4F(1,1,1,0.2f);
                }
            }
        }
        else
        {
            for(int i = 0 ; i < 9 ; ++ i)
            {
                _colors[i] = Color4F(0, 0, 0, 1.0f);
            }
        }
    }
    else
    {
        for(int i = 0 ; i < 9 ; ++ i)
        {
            if ( _rects[i].containsPoint(mouseWorldPosition) ) {
                _colors[i] = Color4F(1, 1, 1, 0.8f);
                
                ImGui::GetIO().MouseDrawCursor = true;
                
                if ( i == 0 || i == 8) ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNWSE);
                else if ( i == 2 || i == 6 ) ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNESW);
                else if ( i == 1 || i == 7 ) ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
                else if ( i == 3 || i == 5 ) ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
                else if ( i == 4 ) ImGui::SetMouseCursor(ImGuiMouseCursor_Move);
            }
            else _colors[i] = Color4F(1, 1, 1, 0.4f);
        }
        
        if ( ImGui::GetIO().MouseClicked[0] )
        {
            if ( !_aabb.containsPoint(mouseWorldPosition) )
            {
                _isSelected = false;
            }
        }
    }
    
    renderOutline();
}


void LocationNode::renderOutline()
{
    _aabbNode->clear();
    
    //  1 2     3 4
    //  ._._____._.
    //  |         |
    // 5'         '6
    //  |         |
    //  |         |
    // 7'         '8
    //  |         |
    // 9._._____._.12
    //    10   11
    
    auto file = _gmxLayer.getFile();
    Size rectSize(file.tileWidth / 4 * _sizeX, file.tileHeight / 4 * _sizeY);
    
    Vec2 p1(1, rectSize.height - 2);
    Vec2 p2(dummySpace, rectSize.height - 2);
    Vec2 p3(rectSize.width - dummySpace, rectSize.height - 2);
    Vec2 p4(rectSize.width - 2, rectSize.height - 2);
    Vec2 p5(1, rectSize.height - dummySpace);
    Vec2 p6(rectSize.width - 2, rectSize.height - dummySpace);
    Vec2 p7(1, dummySpace);
    Vec2 p8(rectSize.width - 2, dummySpace);
    Vec2 p9(1, 1);
    Vec2 p10(dummySpace, 1);
    Vec2 p11(rectSize.width - dummySpace, 1);
    Vec2 p12(rectSize.width - 2, 1);
    
    // left top
    _aabbNode->drawLine(p1, p2, _colors[0]);
    _aabbNode->drawLine(p1, p5, _colors[0]);
    
    // top
    _aabbNode->drawLine(p2, p3, _colors[1]);
    
    // right top
    _aabbNode->drawLine(p3, p4, _colors[2]);
    _aabbNode->drawLine(p4, p6, _colors[2]);
    
    // left
    _aabbNode->drawLine(p5, p7, _colors[3]);
    
    // right
    _aabbNode->drawLine(p6, p8, _colors[5]);
    
    // left bot
    _aabbNode->drawLine(p7, p9, _colors[6]);
    _aabbNode->drawLine(p9, p10, _colors[6]);
    
    // bot
    _aabbNode->drawLine(p10, p11, _colors[7]);
    
    // right bot
    _aabbNode->drawLine(p8, p12, _colors[8]);
    _aabbNode->drawLine(p11, p12, _colors[8]);
    
    // inner rect
    _aabbNode->drawSolidRect(Vec2(2, 2), Vec2(rectSize.width - 2, rectSize.height - 2), Color4F(0.2f, 0.2f, 0.8f, 0.2f));
}


void LocationNode::setLocationSize(int x, int y)
{
    _sizeX = x;
    _sizeY = y;
    
    dummySpace = 10 + std::min(_sizeX, _sizeY);
}


void LocationNode::setPosition(float x, float y)
{
    Node::setPosition(x, y);
    updateRects();
}


void LocationNode::setPosition(const cocos2d::Vec2& pos)
{
    Node::setPosition(pos);
    updateRects();
}






