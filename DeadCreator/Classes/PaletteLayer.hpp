//
//  PaletteLayer.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 22..
//
//

#ifndef PaletteLayer_hpp
#define PaletteLayer_hpp

#include "cocos2d.h"

#include "GMXFile.hpp"

enum PaletteType
{
    DEFAULT = -1,
    TILE,
    HUMAN,
    ITEM,
    DOODAD
};

class PaletteLayer : public cocos2d::Node
{
    
public:
    
    PaletteLayer();
    
    virtual ~PaletteLayer();
    
    static PaletteLayer* create();
    
    void showLayer(bool* opened);
    
    void setPaletteType(PaletteType type) { _paletteType = type; }
    
    void setSelectedItem(int item) { _selectedItem = item; }
    
    int getPaletteType() const { return _paletteType; }
    
    int getSelectedItem() const { return _selectedItem; }
    
private:
    
    cocos2d::Size _layerSize;
    cocos2d::Vec2 _layerPosition;
    cocos2d::Rect _boundingBoxPadding;
    
    int _paletteType = 0;
    int _selectedItem = 0;
    
};

#endif /* PaletteLayer_hpp */
