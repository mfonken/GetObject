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
    if (argc > 1) duration = atoi(argv[1]);
    std::cout << "Running GetObject for " << duration << " seconds." << std::endl;
    Object object;
    object.getAverage();
    char list;
    Mat matList[3];
    std::cout << object.getObject(duration, list, matList[0]) << std::endl;
    //Tester tester;
    //std::cout << tester.display(duration) << std::endl;
    std::cout << "ended" << std::endl;
    return 0;
}
