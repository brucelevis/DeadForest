//
//  PaletteLayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 22..
//
//

#pragma once


#include "cocos2d.h"
#include "GMXFile.hpp"

namespace realtrick
{
    
    class GMXLayer;
    
    enum class PaletteType : int
    {
        DEFAULT = -1,
        TILE = 0,
        HUMAN,
        ITEM,
        DOODAD
    };
        
    class PaletteLayer : public cocos2d::Node
    {
        
    public:
        
        explicit PaletteLayer(GMXLayer& gmxLayer);
        virtual ~PaletteLayer();
        
        static PaletteLayer* create(GMXLayer& gmxLayer);
        void showLayer(bool* opened);
        
        void setPaletteType(PaletteType type) { _paletteType = static_cast<int>(type); }
        void setSelectedItem(int item) { _selectedItem = item; }
        PaletteType getPaletteType() const { return static_cast<PaletteType>(_paletteType); }
        int getSelectedItem() const { return _selectedItem; }
        
    private:
        
        GMXLayer& _gmxLayer;
        
        cocos2d::Size _layerSize;
        cocos2d::Vec2 _layerPosition;
        cocos2d::Rect _boundingBoxPadding;
        
        int _paletteType = 0;
        int _selectedItem = -1;
            
    };
        
}
    
    
    
    
    
    
    
