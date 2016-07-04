//
//  Item9mm.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "EntityBase.hpp"
#include "ui/CocosGUI.h"

class Item9mm : public EntityBase
{
    
public:
    
    Item9mm(GMXLayer2& layer, int id);
    
    virtual ~Item9mm();
    
    static Item9mm* create(GMXLayer2& layer, int id, const std::string& fileName, cocos2d::ui::Widget::TextureResType resType = cocos2d::ui::Widget::TextureResType::LOCAL);
    
    bool init(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType);
    
    virtual Item9mm* clone() const override;
    
private:
    
    std::string _fileName;
    
};
