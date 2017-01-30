//
//  main.cpp
//  OpencvLoadCaffe
//
//  Created by Hiroshi on 11/01/2017.
//  Copyright © 2017 x. All rights reserved.
//

#include "IhsorihGoogLeNetObjectRecognizer.h"
#include <iostream>
#include <time.h>

using namespace std;

int main(int argc, char **argv) {
    // load image
    cv::Mat frame = imread("/Users/hiroshi/Documents/_github/opencv/opencv_contrib/modules/dnn/samples/space_shuttle.jpg");

    // create obj-rec object
    IhsorihGoogLeNetObjectRecognizer objRec;
    objRec.init("/Users/hiroshi/Documents/_github/opencv/opencv_contrib/modules/dnn/samples/bvlc_googlenet.prototxt",
                "/Users/hiroshi/Documents/_github/opencv/bvlc_googlenet.caffemodel",
                "/Users/hiroshi/Documents/_github/opencv/opencv_contrib/modules/dnn/samples/synset_words.txt");
    // do object recognition
    objRec.setImage(frame);
    clock_t start = clock();
    objRec.predict();
    double timeLapse = (clock() - start) / (double)CLOCKS_PER_SEC;
    objRec.putProbBar(frame);
    cout<<endl<<"predict time lapse: "<<timeLapse;
    
    cv::imshow("etst", frame);
    if (waitKey(0) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
    {
        cout << "esc key is pressed by user" << endl;
    }
    
    return 0;
}
