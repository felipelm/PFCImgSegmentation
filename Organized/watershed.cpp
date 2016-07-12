//
//  watershed.cpp
//  openCV Pacote
//
//  Created by Felipe Machado and Thiago Vasconcelos on 03/07/16.
//  Copyright © 2016 Felipe Machado. All rights reserved.
//

#include "handler.hpp"

#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

Mat lab, val, img;
float max_pixel;


bool cmpf(float A, float B, float epsilon = 0.005f)
{
    return (fabs(A - B) < epsilon);
}

const float INIT = 0.0;
int changed = 0;
float new_label = 0.0;
int scan_step2 = 1;
int scan_step3 = 1;
float VMAX, LMAX;
int windowSize = 1;

void step1(int x, int y){
    if(val.at<float>(x,y) != 1){
        //inspecionar os vizinhos do pixel para setar o marcador
        for(int i=-windowSize; i<=windowSize; i++){
            for(int j=-windowSize; j<=windowSize; j++){
                //Casos de borda
                if(!(x + i < 0) &&
                   !(y + j < 0) &&
                   !(x+i > img.rows-1) &&
                   !(y+j > img.cols-1) &&
                   //nao pegar o pixel no meio
                   !(i == 0 && j == 0)){
                    //se algum vizinho for menor val(x,y) = 1
                    if(img.at<float>(x,y) > img.at<float>(x+i,y+j)){
                        val.at<float>(x,y) = 1.0;
                    }
                }
            }
        }
    }
}

void step2(int x, int y){
    if(val.at<float>(x,y) != 1){
        float min= VMAX;
        for(int i=-windowSize; i<=windowSize; i++){
            for(int j=-windowSize; j<=windowSize; j++){
                //Casos de borda
                if(!(x + i < 0) &&
                   !(y + j < 0) &&
                   !(x+i > img.rows-1) &&
                   !(y+j > img.cols-1) &&
                   //nao pegar o pixel no meio
                   !(i == 0 && j == 0)){
                    //min = maior vizinho
                    //encontra o maior valor vizinho de minimo
                    if(img.at<float>(x,y) == img.at<float>(x+i,y+j) &&
                       val.at<float>(x+i,y+j) > 0 &&
                       val.at<float>(x+i,y+j) < min){
                        min = val.at<float>(x+i,y+j);
                    }
                }
            }
        }
        //novo valor do pixel baseado nos minimos vizinhos
        if(min != VMAX &&
           val.at<float>(x,y) != min + 1){

            val.at<float>(x,y) = min+1.0;
            if(min+1.0 > LMAX) LMAX = min+1.0;
            changed = 1;
        }
    }
}

void step3(int x, int y){
    float lmin = LMAX;
    float fmin = img.at<float>(x,y);
    if(cmpf(val.at<float>(x,y) , 0.0)){
        for(int i=-windowSize; i<=windowSize; i++){
            for(int j=-windowSize; j<=windowSize; j++){
                //Casos de borda
                if(!(x + i < 0) &&
                   !(y + j < 0) &&
                   !(x+i > img.rows-1) &&
                   !(y+j > img.cols-1) &&
                   //nao pegar o pixel no meio
                   !(i == 0 && j == 0)){

                    if(cmpf(img.at<float>(x,y), img.at<float>(x+i,y+j)) && lab.at<float>(x+i,y+j) > 0.0 && lab.at<float>(x+i,y+j) < lmin){
                        lmin = lab.at<float>(x+i,y+j);
                    }
                }
            }
        }
        if(cmpf(lmin, LMAX) && cmpf(lab.at<float>(x,y) , 0.0)){
            lmin = ++new_label;
        }
    }else{
        if(cmpf(val.at<float>(x,y), 1.0)){
            for(int i=-windowSize; i<=windowSize; i++){
                for(int j=-windowSize; j<=windowSize; j++){
                    //Casos de borda
                    if(!(x + i < 0) &&
                       !(y + j < 0) &&
                       !(x+i > img.rows-1) &&
                       !(y+j > img.cols-1) &&
                       //nao pegar o pixel no meio
                       !(i == 0 && j == 0)){
                        if(img.at<float>(x+i,y+j)<fmin){
                            fmin = img.at<float>(x+i,y+j);
                        }
                    }
                }
            }

            for(int i=-windowSize; i<=windowSize; i++){
                for(int j=-windowSize; j<=windowSize; j++){
                    //Casos de borda
                    if(!(x + i < 0) &&
                       !(y + j < 0) &&
                       !(x+i > img.rows-1) &&
                       !(y+j > img.cols-1) &&
                       //nao pegar o pixel no meio
                       !(i == 0 && j == 0)){
                        if(cmpf(img.at<float>(x+i,y+j), fmin) && lab.at<float>(x+i,y+j) > 0.0 && lab.at<float>(x+i,y+j) < lmin){
                            lmin = lab.at<float>(x+i,y+j);
                        }
                    }
                }
            }
        }else{
            for(int i=-windowSize; i<=windowSize; i++){
                for(int j=-windowSize; j<=windowSize; j++){
                    //Casos de borda
                    if(!(x + i < 0) &&
                       !(y + j < 0) &&
                       !(x+i > img.rows-1) &&
                       !(y+j > img.cols-1) &&
                       //nao pegar o pixel no meio
                       !(i == 0 && j == 0)){
                        if(cmpf(img.at<float>(x+i,y+j), img.at<float>(x,y)) && cmpf(val.at<float>(x+i,y+j), (val.at<float>(x,y) -1.0)) && lab.at<float>(y+j, x+i)>0 && lab.at<float>(x+i,y+j) < lmin){
                            lmin = lab.at<float>(x+i,y+j);
                        }
                    }
                }
            }
        }
    }

    if(!cmpf(lmin, LMAX) && !cmpf(lab.at<float>(x,y), lmin)){
        lab.at<float>(x,y) = lmin;
        changed = 1;
    }
}

Mat color_watershed(){
    // Gera cores aleatorias
    vector<Vec3b> colors;
    for (size_t i = 0; i < max_pixel; i++)
    {
        int b = theRNG().uniform(10, 255);
        int g = theRNG().uniform(10, 255);
        int r = theRNG().uniform(10, 255);
        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }
    // Cria imagem final
    Mat dst = Mat::zeros(lab.size(), CV_8UC3);

    // Pinta cada area de uma cor
    for (int i = 0; i < lab.rows; i++)
    {
        for (int j = 0; j < lab.cols; j++)
        {
            int index = lab.at<int>(i,j) % (int) max_pixel;
            if (index > 0 && index <= max_pixel)
                dst.at<Vec3b>(i,j) = colors[index-1];
            else
                dst.at<Vec3b>(i,j) = Vec3b(0,0,0);
        }
    }

    return dst;
}

Mat Watershed(Mat imagem, int winSize){
    new_label = 0.0;
    scan_step2 = 1;
    scan_step3 = 1;
    windowSize = winSize;
    img = imagem;
    img.convertTo(img, CV_32FC3);
    VMAX = 100000000000;
    LMAX = 100000000000;
    //inicializar o tamanho da matriz
    lab = img.clone();
    val = img.clone();
    //inicializar os valores
    for(int x = 0; x < img.rows; x++){
        for(int y = 0; y < img.cols; y++){
            lab.at<float>(x,y) = INIT;
            val.at<float>(x,y) = INIT;
        }
    }
    //encontra menor pixel de cada vizinhanca
    for(int x = 0; x < img.rows; x++){
        for(int y = 0; y < img.cols; y++){
            step1(x, y);
        }
    }

    cout<<"Step 2"<<endl;

    //encontrar os plateaus mesmo pixel greyscale a partir dos minimos
    while(scan_step2 == 1){
        changed = 0;
        //scan top left >> bottom right
        for(int x = 0; x < img.rows; x++){
            for(int y = 0; y < img.cols; y++){
                step2(x, y);
            }
        }
        if(changed == 0){
            scan_step2 = 0;
        }else{
            changed = 0;
            //scan bottom right >> top left
            for(int x = img.rows -1; x >= 0; x--){
                for(int y = img.cols -1; y >= 0; y--){
                    step2(x, y);
                }
            }
            if(changed == 0){
                scan_step2 = 0;
            }
        }
    }

    int limite = 0;
    cout<<"Step 3"<<endl;

    while(scan_step3 == 1){
        limite++;
        if(limite == 10) break;
        changed = 0;
        //scan top left >> bottom right
        for(int x = 0; x < img.rows; x++){
            for(int y = 0; y < img.cols; y++){
                step3(x, y);
            }
        }

        if(changed == 0){
            scan_step3 = 0;
        }else{
            changed = 0;
            //scan bottom right >> top left
            for(int x = img.rows -1; x >= 0; x--){
                for(int y = img.cols -1; y >= 0; y--){
                    step3(x, y);
                }
            }
            if(changed == 0){
                scan_step3 = 0;
            }
        }
    }

    max_pixel=0;

    for(int x = 0; x < lab.rows; x++){
        for(int y = 0; y < lab.cols; y++){
            if(max_pixel < lab.at<float>(x,y)) max_pixel = lab.at<float>(x,y);
        }
    }
    lab = color_watershed();
    lab.convertTo(lab, CV_8U,255.0/(max_pixel));

    return lab;
}
