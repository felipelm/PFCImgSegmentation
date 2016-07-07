#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "handler.hpp"

using namespace cv;
using namespace std;


Mat src, srcAux, water, waterFloat; Mat dst1;
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
    Mat src = imread( "/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/lena.jpg");
//    Mat src = imread( "/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/piramida_preta.png");
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
//    erode(mediana_show, mediana_show, kernel1, Point(-1, -1), 10);
//    variancia_show = Dispersao(mediana_show, janela_variancia, thresh_variancia);
//    cout<<"ThreshDisp: "<<thresh_variancia<<" JanDisp: "<<janela_variancia<<" JanMed: "<<janela_mediana<<endl;

    cout<<"ThreshDisp: "<<thresh_dispersao<<" JanDisp: "<<janela_dispersao<<" JanMed: "<<janela_mediana<<endl;
    bitwise_not(mediana_show, mediana_show);
    //    namedWindow( "Variancia", CV_WINDOW_AUTOSIZE ); imshow( "Variancia", variancia_show );
    namedWindow( "Variancia+Mediana", CV_WINDOW_AUTOSIZE );
    imshow( "Variancia+Mediana", mediana_show );



    //Watershed
    output = Watershed(mediana_show, 1);
    output.convertTo(waterFloat, CV_32FC3);

    namedWindow( "Resultado", CV_WINDOW_AUTOSIZE );
    //Cria callback mouse
    setMouseCallback("Resultado", CallBackFunc, NULL);
    imshow( "Resultado", output );
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {

        cout<<"CLICK X: "<< x <<" Y: "<< y << " Valor: "<< waterFloat.at<float>(x,y)<<" Orig: "<<waterFloat.at<float>(x,y)<<endl;
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
        cout<<"X: "<< x <<" Y: "<< y << " Valor: "<< waterFloat.at<float>(y,x)<<endl;

    }
}
