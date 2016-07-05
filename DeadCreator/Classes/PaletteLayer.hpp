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

class GMXLayer2;

enum class PaletteType : int
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
    
    explicit PaletteLayer(GMXLayer2& gmxLayer);
    
    virtual ~PaletteLayer();
    
    static PaletteLayer* create(GMXLayer2& gmxLayer);
    
    void showLayer(bool* opened);
    
    void setPaletteType(PaletteType type) { _paletteType = static_cast<int>(type); }
    
    void setSelectedItem(int item) { _selectedItem = item; }
    
    PaletteType getPaletteType() const { return static_cast<PaletteType>(_paletteType); }
    
    int getSelectedItem() const { return _selectedItem; }
    
private:
    
    GMXLayer2& _gmxLayer;
    
    cocos2d::Size _layerSize;
    cocos2d::Vec2 _layerPosition;
    cocos2d::Rect _boundingBoxPadding;
    
    int _paletteType = 0;
    int _selectedItem = 0;
    
};

#endif /* PaletteLayer_hpp */
