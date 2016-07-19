//
//  EditorItemM16A2.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "EditorEntityBase.hpp"
#include "ui/CocosGUI.h"

namespace realtrick
{
    
    class EditorItemM16A2 : public EditorEntityBase
    {
        
    public:
        
        EditorItemM16A2(GMXLayer& layer, int id);
        
        virtual ~EditorItemM16A2();
        
        static EditorItemM16A2* create(GMXLayer& layer, int id, const std::string& fileName, cocos2d::ui::Widget::TextureResType resType = cocos2d::ui::Widget::TextureResType::LOCAL);
        
        bool init(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType);
        
        
    private:
        
        std::string _fileName;
        
    };
    
}