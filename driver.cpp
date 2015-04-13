//
//  main.cpp
//  recognize
//
//  Created by Matthew Fonken on 1/23/15.
//  Copyright (c) 2015 Matthew Fonken. All rights reserved.
//

#include <iostream>
#include "GetObject.cpp"
#include "CameraTest.cpp"

int main(int argc, const char * argv[]) {
    int count = 30;
    int thresh = 50;
    if (argc > 1) count = atoi(argv[1]);
    if (argc > 2) thresh = atoi(argv[2]);
    std::cout << "Running GetObject for " << count << " seconds at threshold " << thresh << std::endl;
    Object object;
    //object.getAverage();
    char list;
    //for (int i = 0; i < count; i++)
        object.captureImage();
    std::cout << object.getObject(count, list, thresh) << std::endl;
    //std::cout << object.scanImageForCharacter(list,thresh) << std::endl;
    //Tester tester;
    //std::cout << tester.display(count) << std::endl;
    std::cout << "ended" << std::endl;
    return 0;
}
