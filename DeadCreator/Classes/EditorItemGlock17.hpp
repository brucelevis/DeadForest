//
//  EditorItemGlock17.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "EditorEntityBase.hpp"
#include "ui/CocosGUI.h"

namespace realtrick
{
    
    class EditorItemGlock17 : public EditorEntityBase
    {
        
    public:
        
        EditorItemGlock17(GMXLayer2& layer, int id);
        
        virtual ~EditorItemGlock17();
        
        static EditorItemGlock17* create(GMXLayer2& layer, int id, const std::string& fileName, cocos2d::ui::Widget::TextureResType resType = cocos2d::ui::Widget::TextureResType::LOCAL);
        
        bool init(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType);
        
        
    private:
        
        std::string _fileName;
        
    };
    
}