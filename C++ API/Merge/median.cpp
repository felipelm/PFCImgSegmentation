//
//  median.cpp
//  openCV Pacote
//
//  Created by Felipe Machado and Thiago Vasconcelos on 03/07/16.
//  Copyright © 2016 Felipe Machado. All rights reserved.
//

#include "handler.hpp"
#include <opencv2/opencv.hpp>
using namespace cv;

void insertionSort(int window[])
{
    int temp, i , j;
    for(i = 0; i < 9; i++){
        temp = window[i];
        for(j = i-1; j >= 0 && temp < window[j]; j--){
            window[j+1] = window[j];
        }
        window[j+1] = temp;
    }
}

Mat Median(Mat img, unsigned int filterSize){
    Mat imageWithMedianFilter = img.clone();
    int window[filterSize*filterSize];
    for(int y = 0; y < img.rows; y++)
        for(int x = 0; x < img.cols; x++)
            imageWithMedianFilter.at<uchar>(y,x) = 0.0;
    for(int y=filterSize/2; y<imageWithMedianFilter.rows-filterSize/2;y++)
    {
        for(int x = filterSize/2; x < imageWithMedianFilter.cols-filterSize/2; x++)
        {
            for(int i=0; i<filterSize; i++){
                for(int j=0; j<filterSize; j++){
                    window[i*filterSize+j]= img.at<uchar>(y - filterSize/2 + j ,x - filterSize/2 + i);
                }
            }
            insertionSort(window);
            imageWithMedianFilter.at<uchar>(y,x) = window[filterSize/2];
        }
    }
    return imageWithMedianFilter;
}
