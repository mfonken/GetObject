//
//  GetObject.cpp
//  recognize
//
//  Created by Matthew Fonken on 1/23/15.
//  Copyright (c) 2015 Matthew Fonken. All rights reserved.
//

#include "Scan.cpp"
#include "CheckObject.cpp"
#include <fstream>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string>
#include <sys/time.h>

using namespace cv;

class Tester {
    VideoCapture cap;                        //Initalize camera
    int window_width, window_height;
    Mat capture, object;                    //Image containers
    
public:
    Tester();
    bool display(int)
};

Tester::Tester() {
    std::cout << "Finding camera" << std::endl;
    VideoCapture tempCap(0);
    tempCap.getSupported
    tempCap.set(CV_CAP_PROP_FRAME_WIDTH,320);
    tempCap.set(CV_CAP_PROP_FRAME_HEIGHT,240);
    cap = tempCap;
    Mat tempObject(100, 100,  CV_8UC3);
    object = tempObject;
    if(!cap.isOpened()) {
        std::cout << "No camera!" << std::endl;
        return;
    }
    Mat temp;
    cap >> temp;
    std::cout << "cols: " << temp.cols << " | rows: " << temp.rows << std::endl;
    
}

bool Object::display(int duration) {
    std::cout << "Displaying for " << duration << "s:\n";
    timeval tb, te;
    gettimeofday(&tb, NULL);
    int counter = 0;
    gettimeofday(&te, NULL);
    while ((double)(te.tv_sec - tb.tv_sec) < duration) {
        cap >> capture;
        std::cout << "Frame: " << std::endl << capture << std::endl;
    }
}