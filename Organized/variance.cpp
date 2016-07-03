//
//  variance.cpp
//  openCV Pacote
//
//  Created by Felipe Machado and Thiago Vasconcelos on 03/07/16.
//  Copyright © 2016 Felipe Machado. All rights reserved.
//

#include "handler.hpp"
#include <opencv2/opencv.hpp>
using namespace cv;

Mat Variance(Mat img, int filterSize, float threshold){
    Mat varianceImage = img.clone();
    float variance, sum = 0;
    float element, mean;
    for(int y=filterSize/2; y<varianceImage.rows-filterSize/2;y++)
    {
        for(int x = filterSize/2; x < varianceImage.cols-filterSize/2; x++)
        {
            element =0;
            sum =0;
            mean =0;
            for(int i=0; i<filterSize; i++){
                for(int j=0; j<filterSize; j++){
                    element = (float)img.at<uchar>(y - filterSize/2 + j ,x - filterSize/2 + i);
                    mean += element/(filterSize*filterSize);
                    sum +=element*element/(filterSize*filterSize);
                    
                }
            }
            variance = sum - mean*mean;
            if(variance>threshold)    varianceImage.at<uchar>(y,x) = variance;
            
        }
    }
    return varianceImage;
}
