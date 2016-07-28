//
//  StringHelper.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 27..
//
//

#pragma once

namespace
{
    
    template <typename T> static inline std::string _to_string(T number)
    {
        std::ostringstream convStream;
        convStream << number;
        return convStream.str();
    }
    
}









