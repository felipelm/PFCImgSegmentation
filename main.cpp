#include<iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

void insertionSort(float window[])
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

Mat MedianFilter(Mat img, unsigned int filterSize){
    Mat imageWithMedianFilter = img.clone();
    float window[filterSize*filterSize];
    for(int y = 0; y < img.rows; y++)
        for(int x = 0; x < img.cols; x++)
            imageWithMedianFilter.at<uchar>(y,x) = 0.0;
    for(int y=0; y<imageWithMedianFilter.rows;y++)
    {
        for(int x = 0; x < imageWithMedianFilter.cols; x++)
        {
            for(int i=0; i<filterSize; i++){
                for(int j=0; j<filterSize; j++){
                    window[i*filterSize+j]= img.at<uchar>(y - filterSize/2 + j ,x - filterSize/2 + i);
                }
            }
                    //sort the window to find median FILTRO
                    insertionSort(window);

                    //assign the median to centered element of the matrix FILTRO
                    imageWithMedianFilter.at<uchar>(y,x) = window[filterSize/2];
                }
            }
                return imageWithMedianFilter;
            }

    Mat Variance(Mat img, int filterSize, float threshold){
            Mat varianceImage = img.clone();
            float variance, sum = 0;
            float element, mean;
//            for(int y = 0; y < img.rows; y++)
//                for(int x = 0; x < img.cols; x++)
//                    varianceImage.at<uchar>(y,x) = 0.0;
                for(int y=0; y<varianceImage.rows;y++)
                {
                for(int x = 0; x < varianceImage.cols; x++)
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
//                    cout<<variance<<endl;
            if(variance<threshold)	varianceImage.at<uchar>(y,x) = 255;
        }
    }
    return varianceImage;
}

        Mat Laplacian(Mat img){
        //laplaciano
            float window2[9];
        for(int y = 0; y < img.rows - 1; y++){
            for(int x = 0; x < img.cols - 1; x++){

            window2[0] = img.at<uchar>(y - 1 ,x - 1);
            window2[1] = img.at<uchar>(y, x - 1);
            window2[2] = img.at<uchar>(y + 1, x - 1);
            window2[4] = img.at<uchar>(y, x);
            window2[6] = img.at<uchar>(y - 1, x + 1);
            window2[7] = img.at<uchar>(y, x + 1);
            window2[8] = img.at<uchar>(y + 1, x + 1);

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

//        dst = MedianFilter(src, 9);
        dst = Variance(src, 3, 100);

    namedWindow("final" );
    imshow("final", dst);

    namedWindow("inicial");
    imshow("initial", src);

    waitKey();


    return 0;
}
