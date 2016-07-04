//
//  Sheriff.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#pragma once

#include "EntityBase.hpp"
#include "ui/CocosGUI.h"

class Sheriff : public EntityBase
{
    
public:
    
    Sheriff(GMXLayer2& layer, int id);
    
    virtual ~Sheriff();
    
    static Sheriff* create(GMXLayer2& layer, int id, cocos2d::ui::Widget::TextureResType resType);
    
    bool init(cocos2d::ui::Widget::TextureResType resType);
    
private:
    
    
    
};
