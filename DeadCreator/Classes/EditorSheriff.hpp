//
//  EditorSheriff.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#pragma once

#include "EditorEntityBase.hpp"
#include "ui/CocosGUI.h"

namespace realtrick
{
    
    class EditorSheriff : public EditorEntityBase
    {
        
    public:
        
        EditorSheriff(GMXLayer2& layer, int id);
        
        virtual ~EditorSheriff();
        
        static EditorSheriff* create(GMXLayer2& layer, int id, cocos2d::ui::Widget::TextureResType resType);
        
        bool init(cocos2d::ui::Widget::TextureResType resType);
        
    private:
        
        
        
    };
    
}