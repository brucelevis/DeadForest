//
//  CommandBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 3..
//
//

#pragma once

#include <exception>
#include <string>

namespace realtrick
{
    
    class GMXLayer;
    
    class CommandBase
    {
        
    public:
        
        explicit CommandBase(GMXLayer* layer) :
        _layer(layer),
        _isBegan(false)
        {}
        
        CommandBase(const CommandBase& rhs)
        {
            copyFrom(rhs);
        }
        
        void copyFrom(const CommandBase& rhs)
        {
            _layer = rhs._layer;
            _isBegan = rhs._isBegan;
            _commandName = rhs._commandName;
        }
        
        virtual ~CommandBase() { _layer = nullptr; }
        virtual void execute() = 0;
        virtual void undo() = 0;
        virtual CommandBase* clone() const = 0;
        
        virtual void beginImpl() {}
        virtual void endImpl() {}
        
        virtual void begin() final
        {
            if ( _isBegan )
            {
                throw std::runtime_error("Command is already began.");
            }
            
            _isBegan = true;
            beginImpl();
        }
        
        virtual void end() final
        {
            _isBegan = false;
            endImpl();
        }
        
        std::string getCommandName() const { return _commandName; }
        virtual bool empty() const { return true; }
        
    protected:
        
        GMXLayer* _layer;
        bool _isBegan;
        std::string _commandName;
        bool _isEmpty;
        
    };
    
}









