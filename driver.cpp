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
    int duration = 30;
    int thresh = 50;
    if (argc > 1) duration = atoi(argv[1]);
    if (argc > 2) thresh = atoi(argv[2]);
    std::cout << "Running GetObject for " << duration << " seconds at threshold " << thresh << std::endl;
    Object object;
    object.getAverage();
    char list;
    object.captureImage();
    object.captureImage();
    object.captureImage();
    //std::cout << object.getObject(duration, list, matList[0], thresh) << std::endl;
    std::cout << object.scanImageForCharacter(list,thresh) << std::endl;
    //Tester tester;
    //std::cout << tester.display(duration) << std::endl;
    std::cout << "ended" << std::endl;
    return 0;
}
