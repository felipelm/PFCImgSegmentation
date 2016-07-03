#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "handler.hpp"

using namespace cv;
using namespace std;


Mat src, srcAux; Mat dst1;
int thresh_dispersao = 20, janela_dispersao = 3, max_janela_disp = 21;
int thresh_variancia = 7, janela_variancia = 3, max_janela_var = 21;
int janela_mediana=3, max_janela_med = 21;

int max_thresh = 255;
RNG rng(12345);

void thresh_callback(int, void* );

/** @function main */
int main( int argc, char** argv )
{
    Mat src = imread( "/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/chessboard.png");
    srcAux = src.clone();
    cvtColor(srcAux, srcAux, CV_BGR2GRAY);
    String source_window = "Source";
    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    imshow( source_window, src );
    
    //Variancia
    createTrackbar( "Thresh Disp: ", "Source", &thresh_dispersao, max_thresh, thresh_callback);
    createTrackbar( "Janela Disp:", "Source", &janela_dispersao, max_janela_disp, thresh_callback);
    //Dispersao
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
    
    mediana_show = Median(srcAux, janela_mediana);
    dispersao_show = Dispersion(mediana_show, janela_dispersao, thresh_dispersao);
    //    variancia_show = Dispersao(mediana_show, janela_variancia, thresh_variancia);
    output = Watershed(dispersao_show);
    cout<<"ThreshDisp: "<<thresh_dispersao<<" JanDisp: "<<janela_dispersao<<" JanMed: "<<janela_mediana<<endl;
//    cout<<output;
    
    //    namedWindow( "Variancia", CV_WINDOW_AUTOSIZE ); imshow( "Variancia", variancia_show );
    namedWindow( "Dispersao", CV_WINDOW_AUTOSIZE ); imshow( "Dispersao", dispersao_show );
    namedWindow( "Resultado", CV_WINDOW_AUTOSIZE ); imshow( "Resultado", output );
}