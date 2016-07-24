//
//  utils.h
//  TheDeadForestLobbyServer
//
//  Created by ByoungKwon Do on 2016. 5. 27..
//  Copyright © 2016년 ByoungKwon Do. All rights reserved.
//

#ifndef utils_h
#define utils_h


#endif /* utils_h */

namespace realtrick
{
    class Utils{
    private:
        bool isDebug = true;
    public:
        Utils() {};
        ~Utils() {};
        void log(std::string str)  { if(isDebug) std::cout << str << endl; }
    };

}