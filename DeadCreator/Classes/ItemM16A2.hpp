//
//  ItemM16A2.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "EntityBase.hpp"
#include "ui/CocosGUI.h"

class ItemM16A2 : public EntityBase
{
    
public:
    
    ItemM16A2(GMXLayer2& layer, int id);
    
    virtual ~ItemM16A2();
    
    static ItemM16A2* create(GMXLayer2& layer, int id, const std::string& fileName, cocos2d::ui::Widget::TextureResType resType = cocos2d::ui::Widget::TextureResType::LOCAL);
    
    bool init(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType);
    
    
private:
    
    std::string _fileName;
    
};