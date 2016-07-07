//
//  ItemGlock17.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "EntityBase.hpp"
#include "ui/CocosGUI.h"

namespace realtrick
{
    
    class ItemGlock17 : public EntityBase
    {
        
    public:
        
        ItemGlock17(GMXLayer2& layer, int id);
        
        virtual ~ItemGlock17();
        
        static ItemGlock17* create(GMXLayer2& layer, int id, const std::string& fileName, cocos2d::ui::Widget::TextureResType resType = cocos2d::ui::Widget::TextureResType::LOCAL);
        
        bool init(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType);
        
        
    private:
        
        std::string _fileName;
        
    };
    
}