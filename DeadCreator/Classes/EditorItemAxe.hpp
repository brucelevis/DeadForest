//
//  EditorItemAxe.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "EditorEntityBase.hpp"
#include "ui/CocosGUI.h"

namespace realtrick
{
    
    class EditorItemAxe : public EditorEntityBase
    {
        
    public:
        
        EditorItemAxe(GMXLayer2& layer, int id);
        
        virtual ~EditorItemAxe();
        
        static EditorItemAxe* create(GMXLayer2& layer, int id, const std::string& fileName, cocos2d::ui::Widget::TextureResType resType = cocos2d::ui::Widget::TextureResType::LOCAL);
        
        bool init(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType);
        
    private:
        
        std::string _fileName;
        
    };
    
}