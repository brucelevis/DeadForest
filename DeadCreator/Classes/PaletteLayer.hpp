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

class EditScene2;

enum PaletteType
{
    DEFAULT = -1,
    TILE,
    ENTITY,
    ITEM,
    DOODAD
};

class PaletteLayer : public cocos2d::Node
{
    
public:
    
    explicit PaletteLayer(EditScene2& imguiLayer);
    
    virtual ~PaletteLayer();
    
    static PaletteLayer* create(EditScene2& imguiLayer);
    
    void showLayer(bool* opened);
    
    void setPaletteType(PaletteType type) { _paletteType = type; }
    
    void setSelectedItem(int item) { _selectedItem = item; }
    
    int getPaletteType() const { return _paletteType; }
    
    int getSelectedItem() const { return _selectedItem; }
    
private:
    
    EditScene2& _imguiLayer;
    
    int _paletteType = 0;
    
    int _selectedItem = 0;
    
};

#endif /* PaletteLayer_hpp */
