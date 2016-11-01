////
////  CellSpacePartition.hpp
////  DeadCreator
////
////  Created by NamJunHyeon on 2015. 11. 15..
////
////
//
//#pragma once
//
//#include <vector>
//#include <list>
//#include <algorithm>
//
//#include "cocos2d.h"
//#include "Physics.hpp"
//
//namespace realtrick
//{
//    
//    namespace client
//    {
//        
//        class EntityBase;
//        class GameResource;
//        
//        struct Cell
//        {
//            
//            std::list<EntityBase*>                      members;
//            std::vector<realtrick::Polygon>             walls;
//            std::vector<realtrick::Polygon>             simpleWalls;
//            cocos2d::Rect                               boundingBox;
//            
//            Cell(cocos2d::Rect rect): boundingBox(rect) {}
//            
//        };
//        
//        
//        class CellSpacePartition : public cocos2d::Ref
//        {
//            
//        public:
//            
//            static CellSpacePartition* createWithResource(GameResource* res)
//            {
//                auto ret = new (std::nothrow) CellSpacePartition();
//                if ( ret && ret->initWithResource(res) )
//                {
//                    ret->autorelease();
//                    return ret;
//                }
//                CC_SAFE_DELETE(ret);
//                return nullptr;
//            }
//            
//            bool initWithResource(GameResource* res);
//            
//            void                     addWall(const realtrick::Polygon& wall);
//            void                     addSimpleWall(const realtrick::Polygon& wall);
//            
//        private:
//            
//            CellSpacePartition() = default;
//            
//        private:
//            
//            std::vector<Cell>       _cells;
//            float                   _worldWidth;
//            float                   _worldHeight;
//            int                     _numOfCellsX;
//            int                     _numOfCellsY;
//            float                   _cellWidth;
//            float                   _cellHeight;
//            
//        };
//        
//    }
//}
//
//
//
//
//
//
//
//
//
