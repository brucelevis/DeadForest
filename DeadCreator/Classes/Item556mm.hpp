//
//  Item556mm.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "EntityBase.hpp"
#include "ui/CocosGUI.h"

class Item556mm : public EntityBase
{
    
public:
    
    Item556mm(GMXLayer2& layer, int id);
    
    virtual ~Item556mm();
    
    static Item556mm* create(GMXLayer2& layer, int id, const std::string& fileName, cocos2d::ui::Widget::TextureResType resType = cocos2d::ui::Widget::TextureResType::LOCAL);
    
    bool init(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType);
    
private:
    
    
    
};
