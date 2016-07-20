//
//  TileToolCommand.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 3..
//
//

#pragma once

#include <vector>

#include "CommandBase.hpp"
#include "TileBase.hpp"

namespace realtrick
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
        
        virtual void execute() override;
        virtual void undo() override;
        virtual TileToolCommand* clone() const override;
        void pushTile(const TileBase& prevTile, const TileBase& currTile);
        virtual bool empty() const override { return _prevTiles.empty(); }
        
    private:
        
        virtual void beginImpl() override;
        
    private:
        
        std::vector<TileBase> _prevTiles;
        std::vector<TileBase> _currTiles;
        
    };
    
}







