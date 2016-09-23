//
//  color.cpp
//  openCV MergeWatershed
//
//  Created by Felipe Machado on 02/09/16.
//  Copyright © 2016 Felipe Machado. All rights reserved.
//

#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
using namespace cv;
using namespace std;

Mat color(Mat color){
    // Gera cores aleatorias
    vector<Vec3b> colors;
    for (size_t i = 0; i < 255; i++)
    {
        int b = theRNG().uniform(10, 255);
        int g = theRNG().uniform(10, 255);
        int r = theRNG().uniform(10, 255);
        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }
    // Cria imagem final
    Mat dst = Mat::zeros(color.size(), CV_8UC3);
    
    // Pinta cada area de uma cor
    for (int i = 0; i < color.rows; i++)
    {
        for (int j = 0; j < color.cols; j++)
        {
            color.convertTo(color, CV_32F);
            int index = color.at<int>(i,j) % (int) 255;
            if (index > 0 && index <= 255){
                dst.at<Vec3b>(i,j) = colors[index-1];
            }
            else
                dst.at<Vec3b>(i,j) = Vec3b(0,0,0);
        }
    }
    return dst;
}