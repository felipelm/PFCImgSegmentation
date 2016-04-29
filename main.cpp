#include<iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

//ordenar a lista
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

int main()
{
    Mat src, dst;
    
    //Carregar a imagem
    src = imread("/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/lena.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    
    if( !src.data )
    { return -1; }
    //criar uma janela deslisante de tamanho n
    int n = 3;
    
    float window[n*n];
    float window2[n*n];
    
    dst = src.clone();
//    for(int y = 0; y < src.rows; y++)
//        for(int x = 0; x < src.cols; x++)
//            dst.at<uchar>(y,x) = 0.0;
//    
//    for(int y = 0; y < src.rows - 1; y++){
//        for(int x = 0; x < src.cols - 1; x++){
//            //pegar o elemento central
//            //FILTRO
//            for(int a=0; a<n; a++){
//                for(int b=0; b<n; b++){
//                    window[a*n+b] = src.at<uchar>(y - n/2 + b ,x - n/2 + a);
//                }
//            }
//            
//            //SOBEL
//
//            
//            // sort the window to find median FILTRO
//            insertionSort(window);
//            
//            // assign the median to centered element of the matrix FILTRO
//            dst.at<uchar>(y,x) = window[n/2];
//
//            
////            cout<<resultado<<endl;
//        }
//    }
    
    //laplaciano
    
    for(int y = 0; y < dst.rows - 1; y++){
        for(int x = 0; x < dst.cols - 1; x++){
            
            float media;
            
            for(int a=0; a<n; a++){
                for(int b=0; b<n; b++){
                        window2[a*n+b] = src.at<uchar>(y - (float)n/2 + b ,x - (float)n/2 + a);
                }
            }
            
            
            
//            window2[0] = dst.at<uchar>(y - 1 ,x - 1);
//            window2[1] = dst.at<uchar>(y, x - 1);
//            window2[2] = dst.at<uchar>(y + 1, x - 1);
//            window2[3] = dst.at<uchar>(y - 1, x);
//            window2[4] = dst.at<uchar>(y, x);
//            window2[5] = dst.at<uchar>(y + 1, x);
//            window2[6] = dst.at<uchar>(y - 1, x + 1);
//            window2[7] = dst.at<uchar>(y, x + 1);
//            window2[8] = dst.at<uchar>(y + 1, x + 1);
            
            int threshold = 100;
            
            float resultado=window2[0]*(-1)+window2[1]*(-2)+window2[2]*(-1)+window2[6]+window2[7]*(2)+window2[8];
            //threshold
            if(resultado<threshold && resultado>-threshold){
                dst.at<uchar>(y,x) = window2[4];
            }else{
                dst.at<uchar>(y,x) = 128;
            }
        }
    }
    
    
    namedWindow("final" );
    imshow("final", dst);
    
    namedWindow("inicial");
    imshow("initial", src);
    
    waitKey();
    
    
    return 0;
}