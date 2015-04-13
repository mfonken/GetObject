//  ssh 10.31.184.53 -l root
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
    int scan_y_offset; 
    Mat capture, object;                    //Image containers
    int avg[3];
    Mat images[100];
    int image_counter;
public:
    Object();
    bool getAverage();
    bool captureImage();
    bool getObject(int duration, char& characterList, int thresh);
    bool scanImageForCharacter(char& characterList, int thresh);
    int getDiff(Mat&, int&, int&,int*);
};

Object::Object() {
    std::cout << "Finding camera" << std::endl;
    VideoCapture tempCap(-1);
//    tempCap.getSupportedPreviewSizes();
    tempCap.set(CV_CAP_PROP_FRAME_WIDTH,320);
    tempCap.set(CV_CAP_PROP_FRAME_HEIGHT,240);
    //tempCap.set(CV_CAP_PROP_BRIGHTNESS, 255);
    //tempCap.set(CV_CAP_PROP_CONTRAST, 100.0);
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
    
    offset = 10;
    
    samp = 20;                             //sample attributes - cropped immediately after contrast filtering to B&W
    samp_window_width = 300 < temp.cols ? 300:temp.cols;                //sample frame is centered
    samp_window_height = 300 < temp.rows ? 300:temp.rows;
    samp_interval = 30;
    
    object_width_min =  5;                  //object attributes - frame of expected object to be found
    object_width_max =  300;                //only max and min of width and height
    object_height_min =  30;
    object_height_max =  200;
    
    diff_thresh = 100;                       //threshold attribute - applies to contrast filtering, used to check returns form 'color distance' function "getDiff"

    scan_y_offset = 0;
    
    image_counter = 0;
}
bool Object::captureImage() {
    if (image_counter < 100) {
        cap >> images[image_counter++];
        std::cout << "Image counter is " << image_counter << std::endl;
        return true;
    }
    else return false;
}

bool Object::getAverage() {
    //std::cout << "Get Average: ";
    int test = 200;
    timeval tb, te;
    float duration;
    gettimeofday(&tb, NULL);
    cap >> capture;
    
    avg[0] = 0;                                      //...and divide by length to get average
    avg[1] = 0;
    avg[2] = 0;
    
    for (int i = offset; i < samp+offset; i++) {        //Samples on a diagonal from top-left corner plus offset down size of "samp"
        Vec3b tmp_color = capture.at<Vec3b>(Point(offset,i));   //Add up each color channel...
        avg[0] += (int)tmp_color[2];
        avg[1] += (int)tmp_color[1];
        avg[2] += (int)tmp_color[0];
    }
    avg[0] /= samp;                                      //...and divide by length to get average
    avg[1] /= samp;
    avg[2] /= samp;
     
    std::cout << "Background is (" << avg[0] << ", " << avg[1] << ", " << avg[2] << ")" << std::endl;
    gettimeofday(&te, NULL);
    duration = (te.tv_sec - tb.tv_sec) * 1000;      // sec to ms
    duration += (te.tv_usec - tb.tv_usec) / 1000.0;
    //std::cout << duration << "ms" << std::endl;
    if (avg[1] > avg[0] && avg[1] > avg[2]) {
        return false;
    }
    else return true;
}

bool Object::getObject(int duration, char& characterList, int thresh) {
    diff_thresh = thresh;
    //std::cout << "Checking Object for " << duration << "s:\n";
    timeval tb, te, tt;
    gettimeofday(&tb, NULL);
    int counter = 0;
    gettimeofday(&te, NULL);
    while ((double)(te.tv_sec - tb.tv_sec) < duration) {
        getAverage();
        counter++;
        //std::cout << "Loop: " << counter << " | " << (te.tv_sec - tb.tv_sec) + (te.tv_usec - tb.tv_usec)/1000000.0 << "s ";
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
                //video.at<Vec3b>(y,x) = (getDiff(video, x, y, avg) > diff_thresh) ? Vec3b(1,1,1):Vec3b(0,0,0);
                video.at<Vec3b>(y,x) = (getDiff(video, x, y, avg) > diff_thresh) ? Vec3b(0,0,0):Vec3b(255,255,255);
                //video.at<Vec3b>(y,x) = getDiff(video, x, y, avg);
                x++;
            }
            x = 0;
            y++;
        }   //(Yes, for loops would have been easier)
        
        // **********Display Test Start********** //
        /*
        Mat sizeMat(30,30,  CV_8UC3), temp;
        resize(video, sizeMat, sizeMat.size(), INTER_LINEAR);
        cv::cvtColor(sizeMat, temp, CV_BGR2GRAY);
        std::cout << "Frame: " << std::endl << temp << std::endl;
        */
	// **********Display Test End********** //
        x = video.size().width/2;                                              //Reset to first pixel again
        y = video.size().height/2;
        int x_bias = -1;
        int y_bias = 1;
        int x_inc = 0;
        int y_inc = 0;
        bool looper = true;
        char c = '~';                                       //Reset character variable to '~' which acts as null
        
        while (y < video.size().height) {                   //Scan again until...
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
                        std::cout << "o";
                        //rectangle(video, Point(tmp_x_min, tmp_y_min), Point(tmp_x_min+object_width, tmp_y_min+object_height), Scalar(0,255,0)); //draw a rectangle over it
                        //std::cout << "Rectangle made at (" << tmp_x_min << ", " << tmp_y_min << ")" << std::endl;
                        //imshow("Object Found", object);             //display it in another window
                        Mat tmp (video, Rect(tmp_x_min, tmp_y_min, object_width, object_height));   //crop it out onto a new matrix, first a tmp one...
                        resize(tmp, object, object.size(), INTER_LINEAR);                           //...then resize it linearly to correct size (100x100)
                        //cv::cvtColor(object,object, CV_BGR2BW);
                        CheckObject checker;
                        //std::cout << object << std::endl;
                        c = checker.checkChar(object, (float)object_height/(float)object_width);                                                      //Finally check it using "checkChar"
                        //c = absDiff(object);
                        
                        //return true;
                        goto end_loop;                              //If any object is found, program exits scan, would otherwise check every black pixel
                    }
                }
            if (looper) {
                x += x_bias * x_inc * samp_interval;
                y_inc += 1;
                y_bias *= -1;
                looper = !looper;
            }
            else {
                y += y_bias * y_inc * samp_interval;
                x_inc += 1;
                x_bias *= -1;
                looper = !looper;
            }
        }
    end_loop:
        imshow("Filtered", video);                      //End of loop, below is where the string of found chars and "character.txt" are managed
        
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
        gettimeofday(&tt, NULL);
        std::cout << "Loop: " << counter << " | " << (tt.tv_sec - te.tv_sec) + (tt.tv_usec - te.tv_usec)/1000000.0 << "s ";
        if (c != '~')
            std::cout << ": " << c;
        gettimeofday(&te, NULL);
        std::cout << std::endl;
    }
    return false;
}

bool Object::scanImageForCharacter(char& characterList, int thresh) {
    int tmp_counter = 0;
    diff_thresh = thresh;
    //std::cout << "Checking Object for " << duration << "s:\n";
    timeval te, tt;
    int counter = 0;
    gettimeofday(&te, NULL);
    while (tmp_counter < image_counter) {
        getAverage();
        counter++;
        capture = images[tmp_counter++];
        Mat video(capture, Rect((window_width-samp_window_width)/2,(window_height-samp_window_height)/2, samp_window_width, samp_window_height));
        int tmp_x_min = 0,                                  //Reset object location variables for new scan
        tmp_x_max = 1,
        tmp_y_min = 0,
        tmp_y_max = 1;
        int x = 0,                                          //Reset to first pixel coordinate on top left of sample frame
        y = 0;
        while (y < video.size().height) {                   //Dual loop scans row from top to bottom, setting pixels to BGR to B&W based off distance from avgs
            while (x < video.size().width) {                //white if similar, black if unique
                video.at<Vec3b>(y,x) = (getDiff(video, x, y, avg) > diff_thresh) ? Vec3b(0,0,0):Vec3b(255,255,255);
                //video.at<Vec3b>(y,x) = getDiff(video, x, y, avg);
                x++;
            }
            x = 0;
            y++;
        }
        
        // **********Display Test Start********** //
        
        Mat sizeMat(30,30,  CV_8UC3), temp;
        resize(video, sizeMat, sizeMat.size(), INTER_LINEAR);
        cv::cvtColor(sizeMat, temp, CV_BGR2GRAY);
        std::cout << "Frame: " << std::endl << temp << std::endl;
        
        // **********Display Test End********** //
        x = video.size().width/2;                                              //Reset to first pixel again
        y = video.size().height/2;
        int x_bias = -1;
        int y_bias = 1;
        int x_inc = 0;
        int y_inc = 0;
        bool looper = true;
        char c = '~';                                       //Reset character variable to '~' which acts as null
        
        while (y < video.size().height) {                   //Scan again until...
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
                    std::cout << "o";
                    //rectangle(video, Point(tmp_x_min, tmp_y_min), Point(tmp_x_min+object_width, tmp_y_min+object_height), Scalar(0,255,0)); //draw a rectangle over it
                    //std::cout << "Rectangle made at (" << tmp_x_min << ", " << tmp_y_min << ")" << std::endl;
                    //imshow("Object Found", object);             //display it in another window
                    Mat tmp (video, Rect(tmp_x_min, tmp_y_min, object_width, object_height));   //crop it out onto a new matrix, first a tmp one...
                    resize(tmp, object, object.size(), INTER_LINEAR);                           //...then resize it linearly to correct size (100x100)
                    //cv::cvtColor(object,object, CV_BGR2BW);
                    CheckObject checker;
                    //std::cout << object << std::endl;
                    c = checker.checkChar(object, (float)object_height/(float)object_width);                                                      //Finally check it using "checkChar"
                    //c = absDiff(object);
                    
                    //return true;
                    goto end_loop;                              //If any object is found, program exits scan, would otherwise check every black pixel
                }
            }
            if (looper) {
                x += x_bias * x_inc * samp_interval;
                y_inc += 1;
                y_bias *= -1;
                looper = !looper;
            }
            else {
                y += y_bias * y_inc * samp_interval;
                x_inc += 1;
                x_bias *= -1;
                looper = !looper;
            }
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
        gettimeofday(&tt, NULL);
        std::cout << "Loop: " << counter << " | " << (tt.tv_sec - te.tv_sec) + (tt.tv_usec - te.tv_usec)/1000000.0 << "s ";
        if (c != '~')
            std::cout << ": " << c;
        gettimeofday(&te, NULL);
        std::cout << std::endl;
    }
    return false;
}

int Object::getDiff(Mat &video, int &x, int &y, int* background) { //As described above, this is a literal distance formula, given r, g, and b are used as base references
    
    Vec3b tmp_color = video.at<cv::Vec3b>(Point(x,y));
    int tmp_diff = abs(background[2] - tmp_color[0]) + abs(background[1] - tmp_color[1]) + abs(background[0] - tmp_color[2]);
    //std::cout << "Diff: " << tmp_diff << std::endl;
    return tmp_diff;
   
    //std::cout << "O:" << (int)tmp_color[0] << std::endl;
    //int tmp_total = tmp_color[0] + tmp_color[1] + tmp_color[2] + 1;
    ////double scaling_factor = 3*255/tmp_total;
    //int greyness = abs(tmp_color[0]-tmp_color[2])+abs(tmp_color[2]-tmp_color[1])+abs(tmp_color[1]-tmp_color[0]);
    //abs(255 - tmp_color[0]*scaling_factor) + abs(255 - tmp_color[1]*scaling_factor) + abs(255 - tmp_color[2]*scaling_factor);
    //std::cout << "O:" << greyness << std::endl;
    //return greyness;
}
int abs(int n) {
   return (n > 0 ? n:0);
}
