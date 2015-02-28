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

class Object {
    VideoCapture cap;                        //Initalize camera
    int window_width, window_height;
    char last_char, c;
    std::string chars;
    int offset, samp, samp_window_width, samp_window_height, samp_interval;
    int object_width_min, object_width_max, object_height_min, object_height_max;
    int diff_thresh;
    Mat capture, object;                    //Image containers
    int avg[3];
    
public:
    Object();
    void getAverage();
    bool getObject(int, char&);
    float getDiff(Mat&, int&, int&,int*);
};

Object::Object() {
    std::cout << "Finding camera" << std::endl;
    VideoCapture tempCap(0);
//    tempCap.getSupportedPreviewSizes();
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
    window_width = temp.cols;
    window_height = temp.rows;

    last_char = '~';                        //characters last ('~' acts as null)
    
    offset = 50;
    
    samp = 100;                             //sample attributes - cropped immediately after contrast filtering to B&W
    samp_window_width = 300 < temp.cols ? 300:temp.cols;                //sample frame is centered
    samp_window_height = 300 < temp.rows ? 300:temp.rows;
    samp_interval = 50;
    
    object_width_min =  10;                 //object attributes - frame of expected object to be found
    object_width_max =  500;                //only max and min of width and height
    object_height_min =  30;
    object_height_max =  500;
    
    diff_thresh = 10;                      //threshold attribute - applies to contrast filtering, used to check returns form 'color distance' function "getDiff"
}

void Object::getAverage() {
    //std::cout << "Get Average: ";
    timeval tb, te;
    float duration;
    gettimeofday(&tb, NULL);
    cap >> capture;
    memset(avg, 0, sizeof(3));
    for (int i = offset; i < samp+offset; i++) {        //Samples on a diagonal from top-left corner plus offset down size of "samp"
        Vec3b tmp_color = capture.at<Vec3b>(Point(i,offset));   //Add up each color channel...
        avg[0] += (int)tmp_color[2];
        avg[1] += (int)tmp_color[1];
        avg[2] += (int)tmp_color[0];
    }
    avg[0] /= samp;                                      //...and divide by length to get average
    avg[1] /= samp;
    avg[2] /= samp;
    gettimeofday(&te, NULL);
    duration = (te.tv_sec - tb.tv_sec) * 1000;      // sec to ms
    duration += (te.tv_usec - tb.tv_usec) / 1000.0;
    //std::cout << duration << "ms" << std::endl;
}

bool Object::getObject(int duration, char& characterList) {
    std::cout << "Checking Object for " << duration << "s:\n";
    timeval tb, te;
    gettimeofday(&tb, NULL);
    int counter = 0;
    gettimeofday(&te, NULL);
    while ((double)(te.tv_sec - tb.tv_sec) < duration) {
        getAverage();
        counter++;
        std::cout << "Loop: " << counter << " | " << (te.tv_sec - tb.tv_sec) + (te.tv_usec - tb.tv_usec)/1000000.0 << "s\n";
        cap >> capture;
        Mat video(capture, Rect((window_width-samp_window_width)/2,(window_height-samp_window_height)/2, samp_window_width, samp_window_height));
        int tmp_x_min = 0,                                  //Reset object location variables for new scan
        tmp_x_max = 1,
        tmp_y_min = 0,
        tmp_y_max = 1;
        
        int x = 0,                                          //Reset to first pixel coordinate on top left of sample frame
        y = 0;
        while (y < video.size().height) {                   //Dual loop scans row from top to bottom, setting pixels to BGR to B&W based off distance from avgs
            while (x < video.size().width) {                //white if similar, black if unique
                video.at<Vec3b>(y,x) = (getDiff(video, x, y, avg) > diff_thresh) ? Vec3b(255,255,255):Vec3b(0,0,0);
                x++;
            }
            x = 0;
            y++;
        }   //(Yes, for loops would have been easier)
        
        // **********Display Test Start********** //
        Mat sizeMat(50, 50,  CV_8UC3), temp;
        resize(video, sizeMat, sizeMat.size(), INTER_LINEAR);
        //cv::cvtColor(sizeMat, temp, CV_BGR2GRAY);
        std::cout << "Frame: " << std::endl << sizeMat << std::endl;
        // **********Display Test End********** //
        x = 0;                                              //Reset to first pixel again
        y = 0;
        
        char c = '~';                                       //Reset character variable to '~' which acts as null
        
        while (y < video.size().height) {                   //Scan again until...
            while (x < video.size().width) {
                tmp_x_min = 0;
                tmp_x_max = 1;
                tmp_y_min = 0;
                tmp_y_max = 1;
                //imshow("Recognize for Debian", video);
                if ((int)video.at<Vec3b>(Point(x,y))[0] == 0) { //...a black pixel is found
                    //circle(video, Point(x,y), 2, Scalar(0,0,255), 3, 8, 0);
                    Scan scanner;
                    tmp_x_min = (int)scanner.scan(video, 'x', x, y);        //Possible object, scan all directions from this point
                    tmp_x_max = (int)scanner.scan(video, 'X', x, y);        //for continuity to find full object
                    tmp_y_min = (int)scanner.scan(video, 'y', x, y);
                    tmp_y_max = (int)scanner.scan(video, 'Y', x, y);
                    
                    int object_width = tmp_x_max-tmp_x_min;         //Generate width and height of the object
                    int object_height = tmp_y_max-tmp_y_min;
                    //If it fits the defined possible object size...
                    if (object_width > object_width_min && object_width < object_width_max && object_height > object_height_min && object_height < object_height_max) {
                        
                        rectangle(video, Point(tmp_x_min, tmp_y_min), Point(tmp_x_min+object_width, tmp_y_min+object_height), Scalar(0,255,0)); //draw a rectangle over it
                        //cout << "Rectangle made at (" << x-tmp_x_min << ", " << y-tmp_y_min << ")" << endl;
                        //imshow("Object Found", object);             //display it in another window
                        Mat tmp (video, Rect(tmp_x_min, tmp_y_min, object_width, object_height));   //crop it out onto a new matrix, first a tmp one...
                        resize(tmp, object, object.size(), INTER_LINEAR);                           //...then resize it linearly to correct size (100x100)
                        
                        CheckObject checker;
                        c = checker.checkChar(object);                                                      //Finally check it using "checkChar"
                        //c = absDiff(object);
                        if (c != '~')
                            std::cout << "Character: " << c << std::endl;
                        //return true;
                        goto end_loop;                              //If any object is found, program exits scan, would otherwise check every black pixel
                    }
                }
                x+=samp_interval;
            }
            x = 0;
            y+=samp_interval;
        }
    end_loop:
        //imshow("Filtered", video);                      //End of loop, below is where the string of found chars and "character.txt" are managed
        
        if (c != last_char && c != '~') {                           //If a different character is found (and not a '~' local null), c and last_char while be different...
            bool exists = false;                                    //reset temporary flag
            for(int i = 0; i < chars.length(); i++) {               //run through found characters and set flag if this new character has already been found
                if(chars.at(i) == c)                                //!!!This is logical for testing, remove if a single character can be appear more than once!!
                    exists = true;
            }
            if(!exists) {                                           //If the new character is in fact new...
                chars += c;                                         //...add it to the chars list
            }
            last_char = c;                                          //Finally, set this new character to be the last_char
        }
        gettimeofday(&te, NULL);
    }
    return false;
}

float Object::getDiff(Mat &video, int &x, int &y, int* background) { //As described above, this is a literal distance formula, given r, g, and b are used as base references
    Vec3b tmp_color = video.at<cv::Vec3b>(Point(x,y));
    return (tmp_color[2] - background[0] + tmp_color[1] - background[1] + tmp_color[0] - background[2]);
}

    
