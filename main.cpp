#include<iostream>
#include<algorithm>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

Mat MedianFilter(Mat img, unsigned int filterSize){
	Mat imageWithMedianFilter = img.clone();
    vector<float> window(filterSize*filterSize);
    for(int y = 0; y < src.rows; y++)
        for(int x = 0; x < src.cols; x++)
            imageWithMedianFilter.at<uchar>(y,x) = 0.0;
	for(int y=0; y<imageWithMedianFilter.rows;y++)
	{
        for(int x = 0; x < imageWithMedianFilter.cols; x++)
		{
            for(int i=0; i<filterSize; i++){
                for(int j=0; j<filterSize; j++){
                    window.push_back(img.at<uchar>(y - filterSize/2 + j ,x - filterSize/2 + i);
                }
            }
//sort the window to find median FILTRO
            window.sort(window.begin(), window.end());

//assign the median to centered element of the matrix FILTRO
            imageWithMedianFilter.at<uchar>(y,x) = window[n/2];
		}
	}
	return imagWithMedianFilter;
}

Mat Variance(Mat img, unsigned int filterSize, float threshold){
	Mat varianceImage = img.clone();
    float variance, sum = 0;
    for(int y = 0; y < src.rows; y++)
        for(int x = 0; x < src.cols; x++)
            varianceImage.at<uchar>(y,x) = 0.0;
	for(int y=0; y<varianceImage.rows;y++)
	{
        for(int x = 0; x < varianceImage.cols; x++)
		{
            for(int i=0; i<filterSize; i++){
                for(int j=0; j<filterSize; j++){
                	element = (float)img.at<uchar>(y - filterSize/2 + j ,x - filterSize/2 + i)*(1.0-1.0/filterSize)*(float)img.at<uchar>(y - filterSize/2 + j ,x - filterSize/2 + i)*(1.0-1.0/filterSize)
                    variance +=element*element;
                }
            }
            variance/=filterSize;
            if(variance<threshold)	varianceImage = 255;
		}
	}
	return varianceImage;
}

Mat Laplacian(Mat img, unsigned int filterSize){
    //laplaciano
    for(int y = 0; y < dst.rows - 1; y++){
        for(int x = 0; x < dst.cols - 1; x++){
            
            window2[0] = dst.at<uchar>(y - 1 ,x - 1);
            window2[1] = dst.at<uchar>(y, x - 1);
            window2[2] = dst.at<uchar>(y + 1, x - 1);
            window2[6] = dst.at<uchar>(y - 1, x + 1);
            window2[7] = dst.at<uchar>(y, x + 1);
            window2[8] = dst.at<uchar>(y + 1, x + 1);
            
            int threshold = 100;
            
            float resultado=window2[0]*(-1)+window2[1]*(-2)+window2[2]*(-1)+window2[6]+window2[7]*(2)+window2[8];
            //threshold
            if(resultado<threshold && resultado>-threshold){
                img.at<uchar>(y,x) = window2[4];
            }else{
                img.at<uchar>(y,x) = 128;
            }
        }
    }
    return img;
}

int main()
{
    Mat src, dst;
    
    //Carregar a imagem
    src = imread("/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/lena.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    
    if( !src.data )
    { return -1; }
    //criar uma janela deslisante de tamanho n
    
    namedWindow("final" );
    imshow("final", dst);
    
    namedWindow("inicial");
    imshow("initial", src);
    
    waitKey();
    
    
    return 0;
}
