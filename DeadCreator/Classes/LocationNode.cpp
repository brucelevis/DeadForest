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
    
    return true;
}


void LocationNode::setSelected(bool selected)
{
    _isSelected = selected;
    renderOutline();
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
    //   |p' |     |   |   | (DUMMY_CLICK_SPACE)
    //    --- ----- ---    -
    //     6    7    8
    //               |-|
    //             (DUMMY_CLICK_SPACE)
    
    auto file = _gmxLayer.getFile();
    Size rectSize(file.tileWidth / 4 * _sizeX, file.tileHeight / 4 * _sizeY);
    
    Vec2 p = this->getPosition();
    
    _rects[0].setRect(p.x - DUMMY_CLICK_SPACE, p.y + rectSize.height - DUMMY_CLICK_SPACE, DUMMY_CLICK_SPACE * 2, DUMMY_CLICK_SPACE * 2);
    _rects[1].setRect(p.x + DUMMY_CLICK_SPACE, p.y + rectSize.height - DUMMY_CLICK_SPACE, rectSize.width - DUMMY_CLICK_SPACE * 2, DUMMY_CLICK_SPACE * 2);
    _rects[2].setRect(p.x + rectSize.width - DUMMY_CLICK_SPACE, p.y + rectSize.height - DUMMY_CLICK_SPACE, DUMMY_CLICK_SPACE * 2, DUMMY_CLICK_SPACE * 2);
    
    _rects[3].setRect(p.x - DUMMY_CLICK_SPACE, p.y + DUMMY_CLICK_SPACE, DUMMY_CLICK_SPACE * 2, rectSize.height - DUMMY_CLICK_SPACE * 2);
    _rects[4].setRect(p.x + DUMMY_CLICK_SPACE, p.y + DUMMY_CLICK_SPACE, rectSize.width - DUMMY_CLICK_SPACE * 2, rectSize.height - DUMMY_CLICK_SPACE * 2);
    _rects[5].setRect(p.x + rectSize.width - DUMMY_CLICK_SPACE, p.y + DUMMY_CLICK_SPACE, DUMMY_CLICK_SPACE * 2, rectSize.height - DUMMY_CLICK_SPACE * 2);
    
    _rects[6].setRect(p.x - DUMMY_CLICK_SPACE, p.y - DUMMY_CLICK_SPACE, DUMMY_CLICK_SPACE * 2, DUMMY_CLICK_SPACE * 2);
    _rects[7].setRect(p.x + DUMMY_CLICK_SPACE, p.y - DUMMY_CLICK_SPACE, rectSize.width - DUMMY_CLICK_SPACE * 2, DUMMY_CLICK_SPACE * 2);
    _rects[8].setRect(p.x + rectSize.width - DUMMY_CLICK_SPACE, p.y - DUMMY_CLICK_SPACE, DUMMY_CLICK_SPACE * 2, DUMMY_CLICK_SPACE * 2);

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
        if ( ImGui::GetIO().MouseClicked[0] && !_aabb.containsPoint(mouseWorldPosition) )
        {
            _isSelected = false;
        }
        
        for(int i = 0 ; i < 9 ; ++ i)
        {
            if ( _rects[i].containsPoint(mouseWorldPosition) )
            {
                _colors[i] = Color4F(1,1,1,0.8f);
            }
            else
            {
                _colors[i] = Color4F(1,1,1,0.4f);
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
    Vec2 p2(DUMMY_CLICK_SPACE, rectSize.height - 2);
    Vec2 p3(rectSize.width - DUMMY_CLICK_SPACE, rectSize.height - 2);
    Vec2 p4(rectSize.width - 2, rectSize.height - 2);
    Vec2 p5(1, rectSize.height - DUMMY_CLICK_SPACE);
    Vec2 p6(rectSize.width - 2, rectSize.height - DUMMY_CLICK_SPACE);
    Vec2 p7(1, DUMMY_CLICK_SPACE);
    Vec2 p8(rectSize.width - 2, DUMMY_CLICK_SPACE);
    Vec2 p9(1, 1);
    Vec2 p10(DUMMY_CLICK_SPACE, 1);
    Vec2 p11(rectSize.width - DUMMY_CLICK_SPACE, 1);
    Vec2 p12(rectSize.width - 2, 1);
    
    
    _aabbNode->drawLine(p1, p2, _colors[0]);
    _aabbNode->drawLine(p1, p5, _colors[0]);
    
    _aabbNode->drawLine(p2, p3, _colors[1]);
    
    _aabbNode->drawLine(p3, p4, _colors[2]);
    _aabbNode->drawLine(p4, p6, _colors[2]);
    
    _aabbNode->drawLine(p5, p7, _colors[3]);
    
    _aabbNode->drawLine(p6, p8, _colors[5]);
    
    _aabbNode->drawLine(p7, p9, _colors[6]);
    _aabbNode->drawLine(p9, p10, _colors[6]);
    
    _aabbNode->drawLine(p10, p11, _colors[7]);
    
    _aabbNode->drawLine(p8, p12, _colors[8]);
    _aabbNode->drawLine(p11, p12, _colors[8]);
    
    _aabbNode->drawSolidRect(Vec2(2, 2), Vec2(rectSize.width - 2, rectSize.height - 2), Color4F(0.2f, 0.2f, 0.8f, 0.4f));
    
}









