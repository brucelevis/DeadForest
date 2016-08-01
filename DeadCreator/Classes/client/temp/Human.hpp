//
//  Human.hpp
//  DeadCreator
//
//  Created by mac on 2016. 8. 1..
//
//

#include "GameObject2.hpp"

namespace realtrick
{
    namespace client
    {
        
        class Game;
        
        class Human : public GameObject2
        {
            
        public:
            
            Human(Game* game, int id);
            virtual ~Human();
            
            static Human* create(Game* game, int id);
            virtual bool init() override;
            
        };
        
    }
}









