//
//  EditorItemM1897.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "EditorEntityBase.hpp"
#include "ui/CocosGUI.h"

namespace realtrick
{
    
    class EditorItemM1897 : public EditorEntityBase
    {
        
    public:
        
        EditorItemM1897(GMXLayer2& layer, int id);
        
        virtual ~EditorItemM1897();
        
        static EditorItemM1897* create(GMXLayer2& layer, int id, const std::string& fileName, cocos2d::ui::Widget::TextureResType resType = cocos2d::ui::Widget::TextureResType::LOCAL);
        
        bool init(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType);
        
    private:
        
        std::string _fileName;
        
    };
    
}