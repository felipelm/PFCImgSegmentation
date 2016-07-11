#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "handler.hpp"

using namespace cv;
using namespace std;


Mat src, srcAux, water, waterFloat, preProc; Mat dst1;
int thresh_dispersao = 7, janela_dispersao = 4, max_janela_disp = 21;
int thresh_variancia = 3, janela_variancia = 3, max_janela_var = 21;
int janela_mediana=7, max_janela_med = 21;

int max_thresh = 255;
RNG rng(12345);

void thresh_callback(int, void* );
void CallBackFunc(int event, int x, int y, int flags, void* userdata);

/** @function main */
int main( int argc, char** argv )
{
   Mat src = imread( "lena.jpg");
  // Mat src = imread( "testeWatershedmarkers.jpg");
    // Mat src = imread( "bob-esponja.jpg");
//    Mat src = imread( "/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/pb.png");
  //  Mat src = imread( "moedas2.jpg");
  //  Mat src = imread( "piramida_preta.png");
//    Mat src = imread( "/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/piramide.png");
    srcAux = src.clone();
    cvtColor(srcAux, srcAux, CV_BGR2GRAY);
    String source_window = "Source";
    //Mostrar Original
    namedWindow( source_window, CV_WINDOW_AUTOSIZE ); imshow( source_window, src );

//    namedWindow("watershed", 2);
//    srcAux = Variance(srcAux, 3, 10);
//    srcAux = Median(srcAux, 10);
//
//
//    water = Watershed(srcAux, 1);
//
//    water.convertTo(waterFloat, CV_32FC3);
//
//    setMouseCallback("watershed", CallBackFunc, NULL);
//

//    imshow("variancia+mediana", srcAux);
//    imshow( "watershed", water );




    //Dispersao
    createTrackbar( "Thresh Disp: ", "Source", &thresh_dispersao, max_thresh, thresh_callback);
    createTrackbar( "Janela Disp:", "Source", &janela_dispersao, max_janela_disp, thresh_callback);
    //Variancia
//    createTrackbar( "Thresh Variancia:", "Source", &thresh_variancia, max_thresh, thresh_callback);
//    createTrackbar( "Janela Variancia:", "Source", &janela_variancia, max_janela_disp, thresh_callback);
    //Mediana
    createTrackbar( "Janela Mediana:", "Source", &janela_mediana, max_janela_med, thresh_callback);
    thresh_callback( 0, 0 );


    waitKey(0);
    return 0;
}


void thresh_callback(int, void* )
{
    Mat output, dispersao_show, variancia_show, mediana_show;

//    variancia_show = Variance(srcAux, janela_variancia, thresh_variancia);
    dispersao_show = Dispersion(srcAux, janela_dispersao, thresh_dispersao);
    mediana_show = Median(dispersao_show, janela_mediana);

//    Mat kernel1 = Mat::ones(3, 3, CV_8UC1);
//    erode(mediana_show, mediana_show, kernel1, Point(-1, -1), 5);
//    variancia_show = Dispersao(mediana_show, janela_variancia, thresh_variancia);
//    cout<<"ThreshDisp: "<<thresh_variancia<<" JanDisp: "<<janela_variancia<<" JanMed: "<<janela_mediana<<endl;

    cout<<"ThreshDisp: "<<thresh_dispersao<<" JanDisp: "<<janela_dispersao<<" JanMed: "<<janela_mediana<<endl;
    // bitwise_not(mediana_show, mediana_show);

   // kernel pro sharpening
   Mat kernel = (Mat_<float>(3,3) <<
                 1,  1, 1,
                 1, -8, 1,
                 1,  1, 1); // aproximacao da segunda derivada

   // aplicar sharp, laplaciano e truncar numero negativos convertendo pra 0...255 com CV_8U
   Mat imgLaplacian;
   Mat sharp = mediana_show; // copy source image to another temporary one
   filter2D(sharp, imgLaplacian, CV_32F, kernel);
   mediana_show.convertTo(sharp, CV_32F);
   Mat imgResult = sharp - imgLaplacian;
   // convert back to 8bits gray scale
   imgResult.convertTo(imgResult, CV_8UC3);
   imgLaplacian.convertTo(imgLaplacian, CV_8UC3);

   imshow( "Laplaciano", imgLaplacian );
//
//    src = imgResult; // copy back
//    // Imagem binaria da imagem original
//    Mat bw=src.clone();
//
//
    threshold(imgResult, imgResult, 20, 255, CV_THRESH_OTSU);
//
//    Mat dist;
//    distanceTransform(imgLaplacian, imgLaplacian, CV_DIST_L2, 3);
//
//    cin.get();


    //    namedWindow( "Variancia", CV_WINDOW_AUTOSIZE ); imshow( "Variancia", variancia_show );
    namedWindow( "Preproc", CV_WINDOW_AUTOSIZE );
    imgResult.convertTo(preProc, CV_32FC3);
    setMouseCallback("Preproc", CallBackFunc, NULL);
    // Perform the distance transform algorithm
//    Mat dist;
//    distanceTransform(mediana_show, mediana_show, CV_DIST_L2, 3);

    imshow( "Preproc", mediana_show );



    //Watershed
    output = Watershed(mediana_show, 1);
    output.convertTo(waterFloat, CV_32FC3);

//    Mat preprocAux = output.clone();
//    for(int i=0; i< output.rows;i++){
//        for(int j=0; j< output.cols;j++){
//            if(!(j-1 <0)){
//                if(output.at<uchar>(i,j) - output.at<uchar>(i,j-1) < 10 && output.at<uchar>(i,j) != 0) preprocAux.at<uchar>(i,j) = output.at<uchar>(i,j)/10+50;
//            }
//        }
//    }

//    preprocAux.convertTo(waterFloat, CV_32FC3);

    namedWindow( "Resultado", CV_WINDOW_AUTOSIZE );
    //Cria callback mouse
    setMouseCallback("Resultado", CallBackFunc, NULL);

    imshow( "Resultado", output );
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {

        cout<<"CLICK X: "<< x <<" Y: "<< y << " Valor: "<< waterFloat.at<float>(x,y)<<" PreProc: "<<preProc.at<float>(y,x)<<endl;
    }
    else if  ( event == EVENT_RBUTTONDOWN )
    {
        //        cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if  ( event == EVENT_MBUTTONDOWN )
    {
        //        cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if ( event == EVENT_MOUSEMOVE )
    {
        //        cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
        cout<<"X: "<< x <<" Y: "<< y << " Valor: "<< waterFloat.at<float>(y,x)<<" PreProc: "<<preProc.at<float>(y,x)<<endl;;

    }
}
