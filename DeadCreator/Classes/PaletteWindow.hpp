//
//  PaletteWindow.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 22..
//
//

#ifndef PaletteWindow_hpp
#define PaletteWindow_hpp

#include "cocos2d.h"
#include "CCImGui.h"

#include "GMXFile.hpp"

enum PaletteType
{
    DEFAULT = -1,
    TILE,
    ENTITY,
    ITEM,
    DOODAD
};

class PaletteWindow : public cocos2d::Node
{
    
public:
    
    PaletteWindow();
    
    virtual ~PaletteWindow();
    
    static PaletteWindow* create();
    
    void showPaletteWindow(bool* opened);
    
    void setPaletteType(PaletteType type) { _paletteType = type; }
    
    void setSelectedItem(int item) { _selectedItem = item; }
    
    int getPaletteType() const { return _paletteType; }
    
    int getSelectedItem() const { return _selectedItem; }
    
private:
    
    int _paletteType = 0;
    
    int _selectedItem = -1;
    
};

#endif /* PaletteWindow_hpp */
