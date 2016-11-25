//
//  TileToolCommand.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 3..
//
//

#pragma once

#include <vector>

#include "CommandBase.hpp"
#include "Tileset.hpp"
#include "GMXLayer.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class TileToolCommand : public CommandBase
        {
            
        public:
            
            explicit TileToolCommand(GMXLayer* layer) :
            CommandBase(layer)
            {
                _commandName = "Tile Tool";
            }
            
            TileToolCommand(const TileToolCommand& rhs) : CommandBase(rhs)
            {
                copyFrom(rhs);
            }
            
            void copyFrom(const TileToolCommand& rhs)
            {
                _prevTiles = rhs._prevTiles;
                _currTiles = rhs._currTiles;
            }
            
            virtual ~TileToolCommand() = default;
            
            virtual void execute() override
            {
                for( auto tile = _currTiles.cbegin() ; tile != _currTiles.cend() ; ++ tile)
                {
                    _layer->setTile(tile->getIndexX(), tile->getIndexY(), *tile, true);
                }
            }
            
            virtual void undo() override
            {
                for( auto tile = _prevTiles.crbegin() ; tile != _prevTiles.crend() ; ++ tile)
                {
                    _layer->setTile(tile->getIndexX(), tile->getIndexY(), *tile, true);
                }
            }
            
            virtual TileToolCommand* clone() const override { return new TileToolCommand(*this); }
            void pushTile(const Tileset& prevTile, const Tileset& currTile)
            {
                if ( _isBegan )
                {
                    _prevTiles.push_back(prevTile);
                    _currTiles.push_back(currTile);
                }
            }
            
            virtual bool empty() const override { return _prevTiles.empty(); }
            
        private:
            
            virtual void beginImpl() override
            {
                _prevTiles.clear();
                _currTiles.clear();
            }
            
        private:
            
            std::vector<Tileset> _prevTiles;
            std::vector<Tileset> _currTiles;
            
        };
        
    }
}









