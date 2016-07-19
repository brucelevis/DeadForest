//
//  EditorItemShell.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "EditorEntityBase.hpp"
#include "ui/CocosGUI.h"

namespace realtrick
{
    
    class EditorItemShell : public EditorEntityBase
    {
        
    public:
        
        EditorItemShell(GMXLayer& layer, int id);
        
        virtual ~EditorItemShell();
        
        static EditorItemShell* create(GMXLayer& layer, int id, const std::string& fileName, cocos2d::ui::Widget::TextureResType resType = cocos2d::ui::Widget::TextureResType::LOCAL);
        
        bool init(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType);
        
    private:
        
        std::string _fileName;
        
    };
    
}