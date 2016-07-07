//
//  ItemShell.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "EntityBase.hpp"
#include "ui/CocosGUI.h"

namespace realtrick
{
    
    class ItemShell : public EntityBase
    {
        
    public:
        
        ItemShell(GMXLayer2& layer, int id);
        
        virtual ~ItemShell();
        
        static ItemShell* create(GMXLayer2& layer, int id, const std::string& fileName, cocos2d::ui::Widget::TextureResType resType = cocos2d::ui::Widget::TextureResType::LOCAL);
        
        bool init(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType);
        
    private:
        
        std::string _fileName;
        
    };
    
}