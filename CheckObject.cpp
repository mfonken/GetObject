//
//  CheckObject.cpp
//  recognize
//
//  Created by Matthew Fonken on 1/25/15.
//  Copyright (c) 2015 Matthew Fonken. All rights reserved.
//

#include <stdio.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string>

using namespace cv;

class CheckObject {
    float match_thresh, best_match;
    int best_index;
    char c;
public:
    char checkChar(Mat&);
    char checkSimilar(Mat&,std::string);
};

char CheckObject::checkChar(Mat &obj) {                              //Receives only the pointer to image matrix of the object in question
    match_thresh = 0.7;                           //Match threshold, acts as percent
    std::string charList = "ABCDEFGHIJKLMNOPQRSTUVQWXYZ1234567890!@#$%^&*()";   //List of possible objects, each has a corresponding .jpg in the /data folder
    std::string similarChars[] = {"B8&", "I!", "J1", "OQ0", "S3$", "FE", "CG"};  //Collection of similar characters, add freely to this list
    for (int index = 0; index < charList.length (); index++) {                  //Go through each character starting with 'A'
        std::string path("data/");                      //Create path name
        path += charList.at(index);                     //...
        path += ".jpg";                                 //...
        //cout << "Path: " << path << endl;
        IplImage* img = cvLoadImage(path.c_str(),1);    //Load character image
        Mat character(img);                             //Translate into a Mat
        //imshow("Character Image", character);
        int match_count = 0;                            //Reset match counter
        //Mat diff(100, 100,  CV_8UC3);
        for (int i_y = 0; i_y < 100; i_y++) {           //Scan through each image starting from the top-left, count literal pixel matches
            for (int i_x = 0; i_x < 100; i_x++) {
                match_count += (character.at<Vec3b>(Point(i_x,i_y)) == obj.at<Vec3b>(Point(i_x,i_y))) ? 1:0;
            }
        }
        float match = (float)match_count/(100*100);     //Generate % match from the scan counter
        //cout << "% match on " << charList.at(index) << " is " << match << endl;
        char c;
        if (match > match_thresh) {                     //If % match is above the given threshhold
            c = charList.at(index);                     //Get actual character from the charList of the match
            //cout << "Size of similar chars is " << sizeof(similarChars)/sizeof(similarChars[0]) << endl;
            for (int s = 0; s < sizeof(similarChars)/sizeof(similarChars[0]); s++) {    //Run through the similar characters list
                //cout << "Current is " << similarChars[s] << endl;
                for (int k = 0; k < similarChars[s].length(); k++) {
                    if (c == similarChars[s].at(k))     //If the character found is in the list, send it to the checkSimilar function
                        c = checkSimilar(obj, similarChars[s]);
                }
            }
            return c;   //If there are no similar characters to worry about, return as a literal char
        } else {
            //...
        }
        
    }
    return '~';         //If there is no match, return a '~' (local null character)
}

// ********************checkSimilar************************ //
char CheckObject::checkSimilar(Mat &obj, std::string checkList) {    //Receives both the point to the object image matrix and a string of similar chars to check
    //cout << "Check list: " << checkList << endl;
    best_match = 0.0;
    best_index = 0;
    for (int x = 0; x < checkList.length(); x++) {  //Same as checkChar function, however, the object is checked against each character image in the group of similar chars
        std::string path("data/");
        path += checkList.at(x);
        path += ".jpg";
        //cout << "Path: " << path << endl;
        IplImage* img = cvLoadImage(path.c_str(),1);
        Mat character(img);
        int match_count = 0;
        Mat diff(100, 100,  CV_8UC3);
        for (int i_y = 0; i_y < 100; i_y++) {
            for (int i_x = 0; i_x < 100; i_x++) {
                match_count += (character.at<Vec3b>(Point(i_x,i_y)) == obj.at<Vec3b>(Point(i_x,i_y))) ? 1:0;
            }
        }
        if (match_count > best_match) {             //The best match is recorded...
            best_match = match_count;
            best_index = x;
        }
    }
    return checkList.at(best_index);                //...and is return as a literal char
}