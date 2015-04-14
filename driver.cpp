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
#include <sys/time.h>

int main(int argc, const char * argv[]) {
    int count = 30;
    int thresh = 50;
    if (argc > 1) count = atoi(argv[1]);
    if (argc > 2) thresh = atoi(argv[2]);
    std::cout << "Running GetObject for " << count << " seconds at threshold " << thresh << std::endl;
    Object object;
    if (!object.cap.isOpened()) return 0;
    char list;
    for (int i = 0; i < count; i++)
        if(!object.captureImage()) {
            std::cout << "No." << std::endl;
            return 0;
        }
    timeval tb, te;
    gettimeofday(&tb, NULL);
    gettimeofday(&te, NULL);
    while ((double)(te.tv_sec - tb.tv_sec) < 0.01) gettimeofday(&te, NULL);
    //std::cout << object.getObject(count, list, thresh) << std::endl;
    std::cout << object.scanImageForCharacter(list,thresh) << std::endl;
    //Tester tester;
    //std::cout << tester.display(count) << std::endl;
    std::cout << "ended" << std::endl;
    gettimeofday(&tb, NULL);
    gettimeofday(&te, NULL);
    std::cout << "disconnecting camera" << std::endl;
    object.cap.release();
    while ((double)(te.tv_sec - tb.tv_sec) < 1) gettimeofday(&te, NULL);
    return 0;
}
