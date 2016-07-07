//
//  EditorItem556mm.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 4..
//
//

#include "EditorEntityBase.hpp"
#include "ui/CocosGUI.h"

namespace realtrick
{
    
    class EditorItem556mm : public EditorEntityBase
    {
        
    public:
        
        EditorItem556mm(GMXLayer2& layer, int id);
        
        virtual ~EditorItem556mm();
        
        static EditorItem556mm* create(GMXLayer2& layer, int id, const std::string& fileName, cocos2d::ui::Widget::TextureResType resType = cocos2d::ui::Widget::TextureResType::LOCAL);
        
        bool init(const std::string& fileName, cocos2d::ui::Widget::TextureResType resType);
        
    private:
        
        std::string _fileName;
        
    };
    
}