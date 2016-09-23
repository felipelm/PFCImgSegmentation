#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include "handler.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    char *p=argv[0];
    int threshold= 7, repetitions=1, thresholdDisp=3, windowDisp=3, windowMed=0;
    string origin="", destination="", src_dispersion="";
    for(int i=0; i<argc; i++){
        p = argv[i];
        if(strcmp(p, "-tD") == 0){
            thresholdDisp = atoi(argv[i+1]);
        }
        if(strcmp(p, "-fD") == 0){
            windowDisp = atoi(argv[i+1]);
        }
        if(strcmp(p, "-fM") == 0){
            windowMed = atoi(argv[i+1]);
        }
        if(strcmp(p, "-t") == 0){
            threshold = atoi(argv[i+1]);
        }
        if(strcmp(p, "-r") == 0){
            repetitions = atoi(argv[i+1]);
        }
        if(strcmp(p, "-o") == 0){
            origin = argv[i+1];
        }
        if(strcmp(p, "-d") == 0){
            destination = argv[i+1];
        }
    }
    if(origin.compare("") == 0 || destination.compare("") == 0){
        cout<<"NO DESTINATION OR ORIGIN OR DISPERSION"<<endl;
        return -1;
    }
    
    Mat src = imread(origin);
    cvtColor(src, src, CV_BGR2GRAY);
    Mat similarity = Dispersion(src, windowDisp, thresholdDisp);
    Mat median = Median(similarity, windowMed);
    
    if(windowMed != 0){
        similarity = Median(similarity, windowMed);
    }
    
    Mat result;
    result = Watershed(similarity, 1);
    
    for(int i = 0; i<repetitions;i++)
        result = mergeRegion(result, threshold, similarity);
    
    result = color(result);
    imwrite(destination,result);
    return 0;
}