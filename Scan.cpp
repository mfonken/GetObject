//
//  scan.cpp
//  recognize
//
//  Created by Matthew Fonken on 1/25/15.
//  Copyright (c) 2015 Matthew Fonken. All rights reserved.
//

#include <stdio.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

class Scan {
    int scan_size_x_max,      //x is left
    scan_size_y_max,      //X is right
    scan_size_X_max,      //y is up
    scan_size_Y_max,      //Y is down
    scan_thresh,           //Number of required black pixels per line scan - if less, scan is "blank" and gap count is incremented
    gap_thresh,             //Number of consecutive blank scans allowed
    gap_count;              //Blank scan counter
    bool gap;               //reset gap flag
public:
    float scan(Mat&, char, int&, int&);
};

float Scan::scan(Mat& video, char dir, int &x, int &y) {
    gap = false;
    int tmp_x = x,                  //temporary coordinates used by each scan
        tmp_y = y;
    scan_size_x_max = 100;      //x is left
    scan_size_y_max = 100;      //X is right
    scan_size_X_max = 200;      //y is up
    scan_size_Y_max = 300;      //Y is down
    scan_thresh = 15;           //Number of required black pixels per line scan - if less, scan is "blank" and gap count is incremented
    gap_thresh = 2;             //Number of consecutive blank scans allowed
    gap_count = 0;              //Blank scan counter
    switch(dir) {
        case 'x':
            for (tmp_x = x; tmp_x > x-scan_size_x_max && !gap && tmp_x > 0; tmp_x--) { //Outer loop is the main scan - reacts to the blank scans given by the inner loop
                int scan_count = 0; //reset blank scan count                           Inner loop scans along perpedicular direction until max or border, counting black pixels
                for (int scan_y = (tmp_y - scan_size_y_max >= 0) ? tmp_y - scan_size_y_max:0; scan_y < tmp_y + scan_size_Y_max && scan_y < video.size().height; scan_y++) {
                    if ((int)video.at<Vec3b>(Point(tmp_x,scan_y))[0] == 0)
                        scan_count++;
                }
                if (scan_count < scan_thresh)                   //If scan is blank, increment gap_count
                    gap_count++;
                else
                    gap_count = 0;                              //Else reset, only checks consecutive blank scans
                if (gap_count > gap_thresh)                     //If there is a gap, the gap flag is set, the loop ends and the current tmp_x is returned
                    gap = true;                                 //This is the same basic algorithm for each direction
            }
            return tmp_x;
        case 'X':   //right
            for (tmp_x = x; tmp_x < x+scan_size_X_max && !gap && tmp_x < video.size().width; tmp_x++) {
                int scan_count = 0;
                for (int scan_y = (tmp_y - scan_size_y_max >= 0) ? tmp_y - scan_size_y_max:0; scan_y < tmp_y + scan_size_Y_max && scan_y < video.size().height; scan_y++) {
                    if ((int)video.at<Vec3b>(Point(tmp_x,scan_y))[0] == 0)
                        scan_count++;
                }
                if (scan_count < scan_thresh)
                    gap_count++;
                else
                    gap_count = 0;
                if (gap_count > gap_thresh)
                    gap = true;
            }
            return tmp_x;
        case 'y':   //up
            for (tmp_y = y; tmp_y > y-scan_size_y_max && !gap && tmp_y > 0; tmp_y--) {
                int scan_count = 0;
                for (int scan_x = (tmp_x - scan_size_x_max >= 0) ? tmp_x - scan_size_x_max:0; scan_x < tmp_x + scan_size_X_max && scan_x < video.size().width; scan_x++) {
                    if ((int)video.at<Vec3b>(Point(scan_x,tmp_y))[0] == 0)
                        scan_count++;
                }
                if (scan_count < scan_thresh)
                    gap_count++;
                else
                    gap_count = 0;
                if (gap_count > gap_thresh)
                    gap = true;
            }
            return tmp_y;
        case 'Y':   //down
            for (tmp_y = y; tmp_y < y+scan_size_Y_max && !gap && tmp_y < video.size().height; tmp_y++) {
                int scan_count = 0;
                for (int scan_x = (tmp_x - scan_size_x_max >= 0) ? tmp_x - scan_size_x_max:0; scan_x < tmp_x + scan_size_X_max && scan_x < video.size().width; scan_x++) {
                    if ((int)video.at<Vec3b>(Point(scan_x,tmp_y))[0] == 0)
                        scan_count++;
                }
                if (scan_count < scan_thresh)
                    gap_count++;
                else
                    gap_count = 0;
                if (gap_count > gap_thresh)
                    gap = true;
            }
            return tmp_y;
        default:
            return 0;
    }
}