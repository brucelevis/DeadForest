#pragma once

#include <sstream>

#include "CellSpacePartition.hpp"
#include "GameMap.hpp"
#include "EntityHuman.hpp"

#define Z_ORDER_GAME_MAP    0
#define Z_ORDER_SHADOW      1
#define Z_ORDER_LIGHT       2
#define Z_ORDER_ITEMS       3
#define Z_ORDER_HUMAN       4
#define Z_ORDER_UI          10


#define BF_MULTIPLY         { GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA }
#define BF_ADDITIVE         { GL_ONE, GL_ONE }

namespace realtrick
{
    
    class EntityBase;
    class EntityHuman;
    class GameWorld;
    class RenderTarget;

    class GameManager
    {
    public:

		GameManager()
		{};

		~GameManager() {};

        
        CellSpacePartition*                     getCellSpace() const { return _cellSpace; }
        
        template <typename T> static inline std::string _to_string(T number)
        {
            std::ostringstream convStream;
            convStream << number;
            return convStream.str();
        }
        
	private:
        
        CellSpacePartition*                         _cellSpace;
    
    };
    
}












