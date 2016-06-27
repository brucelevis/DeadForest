//
//  TerrainBase.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 28..
//
//

#ifndef TerrainBase_hpp
#define TerrainBase_hpp

#include <string>


class TerrainBase
{
    
public:
    
    TerrainBase(int type, const std::string& number) : _type(type), _number(number)
    {}
    
    virtual ~TerrainBase() = default;
    
    int getType() const { return _type; }
    
    std::string getNumber() const { return _number; }
    
private:
    
    int _type;
    
    std::string _number;
    
};


#endif /* TerrainBase_hpp */
