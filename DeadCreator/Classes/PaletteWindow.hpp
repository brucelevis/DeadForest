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

#include "GMXFile.hpp"

class ImGuiLayer;

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
    
    explicit PaletteWindow(ImGuiLayer* imguiLayer);
    
    virtual ~PaletteWindow();
    
    static PaletteWindow* create(ImGuiLayer* imguiLayer);
    
    void showPaletteWindow(bool* opened);
    
    void setPaletteType(PaletteType type) { _paletteType = type; }
    
    void setSelectedItem(int item) { _selectedItem = item; }
    
    int getPaletteType() const { return _paletteType; }
    
    int getSelectedItem() const { return _selectedItem; }
    
private:
    
    ImGuiLayer* _layer;
    
    int _paletteType = 0;
    
    int _selectedItem = 0;
    
};

#endif /* PaletteWindow_hpp */
