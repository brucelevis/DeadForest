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
    class GMXFile;
    
    class LocationNode : public cocos2d::Node
    {
        
    public:
        
        float dummySpace = 10.0f;
        
    public:
        
        explicit LocationNode(GMXLayer2& layer);
        virtual ~LocationNode();
        
        static LocationNode* create(GMXLayer2& layer);
        virtual bool init() override;
        
        void setLocationSize(int x, int y);
        std::pair<int, int> getLocationSize() const { return { _sizeX, _sizeY }; }
        
        void updateRects();
        void update(const cocos2d::Vec2& mouseWorldPosition);
        
        void renderOutline();
        
        virtual void setPosition(float x, float y) override;
        virtual void setPosition(const cocos2d::Vec2& pos) override;
        
    private:
        
        std::function<bool(int offsetX, int offsetY)> resizeFuncs[9];
        void clearLocationGrabFlags();
        bool isOnlyGrabbedLocation(int index);
        
    private:
        
        GMXLayer2& _gmxLayer;
        const GMXFile& _file;
        
        cocos2d::Rect _aabb;
        cocos2d::Rect _rects[9];
        cocos2d::Color4F _colors[9];
        
        cocos2d::DrawNode* _aabbNode;
        cocos2d::ui::Text* _name;
        
        int _sizeX;
        int _sizeY;
        
        bool _isLocationGrabbed[9];
        
        std::pair<int, int> _oldRectangleIndex = {0, 0};
    
    };
    
}









