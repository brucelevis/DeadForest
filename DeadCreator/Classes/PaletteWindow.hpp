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

class PaletteWindow : public cocos2d::Node
{
    
public:
    
    PaletteWindow();
    
    virtual ~PaletteWindow();
    
    static PaletteWindow* create();
    
    void showPaletteWindow(bool* opened);
    
private:
    
    
    
};

#endif /* PaletteWindow_hpp */
