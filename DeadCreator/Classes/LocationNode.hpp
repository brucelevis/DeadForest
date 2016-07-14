//
//  LocationNode.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 14..
//
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace realtrick
{
    
    class GMXLayer2;
    
    class LocationNode : public cocos2d::Node
    {
        
    public:
        
        const float DUMMY_CLICK_SPACE = 10.0f;
        
    public:
        
        explicit LocationNode(GMXLayer2& layer);
        virtual ~LocationNode();
        
        static LocationNode* create(GMXLayer2& layer);
        virtual bool init() override;
        
        bool isSelected() const { return _isSelected; }
        void setSelected(bool selected);
        
        void setLocationSize(int x, int y) { _sizeX = x; _sizeY = y; }
        std::pair<int, int> getLocationSize() const { return { _sizeX, _sizeY }; }
        
        void updateRects();
        void update(const cocos2d::Vec2& mouseWorldPosition);
        
        void renderOutline();
        
    private:
        
        GMXLayer2& _gmxLayer;
        
        cocos2d::Rect _aabb;
        cocos2d::Rect _rects[9];
        cocos2d::Color4F _colors[9];
        
        cocos2d::DrawNode* _aabbNode;
        cocos2d::ui::Text* _name;
        
        int _sizeX = 14;
        int _sizeY = 7;
        
        bool _isSelected = false;
        
    };
    
}









