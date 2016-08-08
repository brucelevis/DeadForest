//
//  EditorType.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 30..
//
//

#pragma once

namespace realtrick
{
    namespace editor
    {
        
        enum class PaletteType : int
        {
            DEFAULT = -1,
            TILE = 0,
            HUMAN,
            ITEM,
        };
            
        enum class LayerType : int
        {
            TILE = 0,
            ENTITY,
            LOCATION,
        };
                
    }
}









