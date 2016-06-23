//
//  Tiling.hpp
//  DeadCreator
//
//  Created by mac on 2016. 6. 23..
//
//

#ifndef Tiling_hpp
#define Tiling_hpp


enum TileType
{
    DIRT = 0,
    GRASS,
    WATER,
    HILL
};

struct Tiling
{
    int x;
    int y;
    std::string tileNumber;
    
    Tiling(int xx = 0, int yy = 0, const std::string& s = "") :
    x(xx),
    y(yy),
    tileNumber(s)
    {}
    
    Tiling(const Tiling& rhs)
    {
        copyFrom(rhs);
    }
    
    Tiling& operator=(const Tiling& rhs)
    {
        if ( &rhs == this )
            return *this;
        copyFrom(rhs);
        return *this;
    }
    
    void copyFrom(const Tiling& src)
    {
        x = src.x;
        y = src.y;
        tileNumber = src.tileNumber;
    }
    
};


struct ModifiedData
{
    int x;
    int y;
    std::string prev;
    std::string curr;
    
    ModifiedData() = default;
    
    ModifiedData(int xx, int yy, const std::string& p, const std::string& c) : x(xx), y(yy), prev(p), curr(c)
    {}
};


#endif /* Tiling_hpp */
