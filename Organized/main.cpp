#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "handler.hpp"

using namespace cv;
using namespace std;


Mat src, srcAux, water, waterFloat, preProc; Mat dst1;
int thresh_dispersao = 6, janela_dispersao = 7, max_janela_disp = 15;
int thresh_variancia = 5, janela_variancia = 3, max_janela_var = 21;
int janela_mediana=8, max_janela_med = 21;
int thresh_merge = 12000, max_merge = 17000;
int num_merge = 1, max_num_merge = 15;


int max_thresh = 40;
RNG rng(12345);

void thresh_callback(int, void* );
void CallBackFunc(int event, int x, int y, int flags, void* userdata);
Mat color_watershed(Mat color);

/** @function main */
int main( int argc, char** argv )
{
    src = imread( "/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/lena.jpg");
//  src = imread( "/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/bob-esponja.jpg");
//  src = imread( "/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/pb.png");
//  src = imread( "/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/moedas2.jpg");
//  src = imread( "/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/pb3.png");
//  src = imread( "/Users/felipemachado/Documents/OpenCV/openCV DragLena/openCV NEW PROJECT/woodbath.png");
//  src = imread( "/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/piramide.png");
//  src = imread( "/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/piramida_preta.png");
//  src = imread( "/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFCa/LAB_IR_2009.jpg");
//  src = imread( "/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/banana1.bmp");
//  src = imread( "/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/BRICK.jpg");
//  src = imread( "/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/CAMOUFLAGE_IR_1880.jpg");
//  src = imread( "/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/Cosine_wave4.png");

    srcAux = src.clone();
    cvtColor(srcAux, srcAux, CV_BGR2GRAY);
    String source_window = "Source";
    //Mostrar Original
    namedWindow( source_window, CV_WINDOW_AUTOSIZE ); imshow( source_window, src );

    //Merge
    createTrackbar( "Thresh Merge: ", "Source", &thresh_merge, max_merge, thresh_callback);
    createTrackbar( "Num Merge: ", "Source", &num_merge, max_num_merge, thresh_callback);
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

    cout<<"ThreshDisp: "<<thresh_dispersao<<" JanDisp: "<<janela_dispersao<<" JanMed: "<<janela_mediana<<endl;


    mediana_show.convertTo(preProc, CV_32FC3);


    imshow( "Preproc", mediana_show );

    //WatershedOpencv
    output = watershedOpencv(src, srcAux);

    //Watershed
//    output = Watershed(mediana_show, 1);

    //Pos-processamento
//    Mat aux =output.clone();
//    for(int i=0;i<num_merge; i++) output = mergeRegion(output, thresh_merge, dispersao_show);
//    output.convertTo(waterFloat, CV_32FC3);

    //Colorir
//    output = color_watershed(output);

    namedWindow( "Resultado", CV_WINDOW_AUTOSIZE );
    //Cria callback mouse
    setMouseCallback("Resultado", CallBackFunc, NULL);


    imshow( "Resultado", output );

    //Salvar imagem
//    imwrite("/Users/felipemachado/Documents/OpenCV/watershedGreyscale.png",output);
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {

        cout<<"CLICK X: "<< x <<" Y: "<< y << " Valor: "<< waterFloat.at<float>(y,x)<<" PreProc: "<<preProc.at<float>(y,x)<<endl;
    }
    else if ( event == EVENT_MOUSEMOVE )
    {
        //        cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
//        cout<<"X: "<< x <<" Y: "<< y << " Valor: "<< waterFloat.at<float>(y,x)<<" PreProc: "<<preProc.at<float>(y,x)<<endl;;

    }
}


Mat color_watershed(Mat color){
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
