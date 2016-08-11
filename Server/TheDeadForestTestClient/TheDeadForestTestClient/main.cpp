//
//  main.cpp
//  TheDeadForestTestClient
//
//  Created by ByoungKwon Do on 2016. 6. 20..
//  Copyright © 2016년 ByoungKwon Do. All rights reserved.
//

#include "Client.hpp"

using namespace realtrick;
using namespace std;

int main()
{
    
    try {
        Client *c = new Client();
        c->run();
    }
    catch (std::exception& e) {
        cout << e.what() << endl;
    }
    
    return 0;
}

