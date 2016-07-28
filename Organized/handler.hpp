//
//  handler.hpp
//  openCV Pacote
//
//  Created by Felipe Machado and Thiago Vasconcelos on 03/07/16.
//  Copyright © 2016 Felipe Machado. All rights reserved.
//

#ifndef handler_hpp
#define handler_hpp

#include <opencv2/opencv.hpp>
using namespace cv;

Mat Dispersion(Mat img, int filterSize, float threshold);
Mat Median(Mat img, unsigned int filterSize);
Mat Variance(Mat img, int filterSize, float threshold);
Mat Watershed(Mat imagem, int winSize);
Mat mergeRegion(Mat labImg, int threshold, Mat dispersion);
Mat watershedOpencv(Mat src, Mat srcAux);

#endif /* handler_hpp */
