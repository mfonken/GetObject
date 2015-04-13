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
    if (!object.cap.isOpened()) return;
    char list;
    if (object.getAverage()) {
        for (int i = 0; i < count; i++)
            object.captureImage();
    }
    else {
        std::cout << "No." << std::endl;
    }
    timeval tb, te;
    gettimeofday(&tb, NULL);
    gettimeofday(&te, NULL);
    while ((double)(te.tv_sec - tb.tv_sec) < 5) gettimeofday(&te, NULL);
    //std::cout << object.getObject(count, list, thresh) << std::endl;
    std::cout << object.scanImageForCharacter(list,thresh) << std::endl;
    //Tester tester;
    //std::cout << tester.display(count) << std::endl;
    std::cout << "ended" << std::endl;
    return 0;
}
