//
//  main.cpp
//  recognize
//
//  Created by Matthew Fonken on 1/23/15.
//  Copyright (c) 2015 Matthew Fonken. All rights reserved.
//

#include <iostream>
#include "GetObject.cpp"

int main(int argc, const char * argv[]) {
    std::cout << "...\n";
    Object object;
    object.getAverage();
    char list;
    std::cout << object.getObject(5,list) << std::endl;
    return 0;
}
