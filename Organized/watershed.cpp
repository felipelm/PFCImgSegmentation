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


Mat lab, val, img;

bool cmpf(float A, float B, float epsilon = 0.005)
{
    return (fabs(A - B) < epsilon);
}

const float INIT = 0.0;
int changed = 0;
int new_label = 0;
int scan_step2 = 1;
int scan_step3 = 1;
float VMAX, LMAX;

void step1(int x, int y){
    if(val.at<float>(x,y) != 1.0){
        //inspecionar os vizinhos do pixel para setar o marcador
        for(int i=-1; i<=1; i++){
            for(int j=-1; j<=1; j++){
                //Casos de borda
                if(x == 0 && i < 0) continue;
                if(y == 0 && j < 0) continue;
                if(x == img.rows-1 && i > 0) continue;
                if(y == img.cols-1 && j > 0) continue;
                //nao pegar o pixel no meio
                if(i == 0 && j == 0) continue;
                //se algum vizinho for menor val(x,y) = 1
                if(img.at<float>(x,y) > img.at<float>(x+i,y+j)) val.at<float>(x,y) = 1.0;
            }
        }
    }
}

void step2(int x, int y){
    if(val.at<float>(x,y) != 1){
        float min= VMAX;
        
        for(int i=-1; i<=1; i++){
            for(int j=-1; j<=1; j++){
                //Casos de borda
                if(x == 0 && i < 0) continue;
                if(y == 0 && j < 0) continue;
                if(x == img.rows-1  && i > 0) continue;
                if(y == img.cols-1 && j > 0) continue;
                //nao pegar o pixel no meio
                if(i == 0 && j == 0) continue;
                //min = maior vizinho
                //encontra o maior valor vizinho de minimo
                if(img.at<float>(x,y) == img.at<float>(x+i,y+j) && val.at<float>(x+i,y+j) > 0 && val.at<float>(x+i,y+j) < min){
                    min = val.at<float>(x+i,y+j);
                }
            }
            
        }
        //novo valor do pixel baseado nos minimos vizinhos
        if(min != VMAX && val.at<float>(x,y) != min + 1 && val.at<float>(x,y) < min && val.at<float>(x,y)==0){
            val.at<float>(x,y) = min+1.0;
            if(min+1.0 > LMAX) LMAX = min+1.0;
            changed = 1;
        }
    }
}

void step3(int x, int y){
    float lmin = LMAX;
    float fmin = img.at<float>(x, y);
    if(cmpf(val.at<float>(x,y) , 0.0)){
        
        for(int i=-1; i<=1; i++){
            for(int j=-1; j<=1; j++){
                //Casos de borda
                if(x == 0 && i < 0) continue;
                if(y == 0 && j < 0) continue;
                if(x == img.rows-1  && i > 0) continue;
                if(y == img.cols-1 && j > 0) continue;
                //nao pegar o pixel no meio
                if(i == 0 && j == 0) continue;
                
                if(cmpf(img.at<float>(x,y), img.at<float>(x+i,y+j)) && lab.at<float>(x+i,y+j) > 0.0 && lab.at<float>(x+i,y+j) < lmin){
                    lmin = lab.at<float>(x+i,y+j);
                }
            }
        }
        if(cmpf(lmin, LMAX) && cmpf(lab.at<float>(x,y) , 0.0)){
            lmin = ++new_label;
        }
        
    }else{
        if(cmpf(val.at<float>(x,y), 1.0)){
            for(int i=-1; i<=1; i++){
                for(int j=-1; j<=1; j++){
                    //Casos de borda
                    if(x == 0 && i < 0) continue;
                    if(y == 0 && j < 0) continue;
                    if(x == img.rows-1  && i > 0) continue;
                    if(y == img.cols-1 && j > 0) continue;
                    //nao pegar o pixel no meio
                    if(i == 0 && j == 0) continue;
                    
                    if(img.at<float>(x+i,y+j)<fmin){
                        fmin = img.at<float>(x+i,y+j);
                    }
                }
            }
            
            for(int i=-1; i<=1; i++){
                for(int j=-1; j<=1; j++){
                    //Casos de borda
                    if(x == 0 && i < 0) continue;
                    if(y == 0 && j < 0) continue;
                    if(x == img.rows-1  && i > 0) continue;
                    if(y == img.cols-1 && j > 0) continue;
                    //nao pegar o pixel no meio
                    if(i == 0 && j == 0) continue;
                    
                    if(cmpf(img.at<float>(x+i,y+j), fmin) && lab.at<float>(x+i,y+j) > 0.0 && lab.at<float>(x+i,y+j) < lmin){
                        lmin = lab.at<float>(x+i,y+j);
                    }
                }
            }
        }else{
            for(int i=-1; i<=1; i++){
                for(int j=-1; j<=1; j++){
                    //Casos de borda
                    if(x == 0 && i < 0) continue;
                    if(y == 0 && j < 0) continue;
                    if(x == img.rows-1  && i > 0) continue;
                    if(y == img.cols-1 && j > 0) continue;
                    //nao pegar o pixel no meio
                    if(i == 0 && j == 0) continue;
                    
                    if(cmpf(img.at<float>(x+i,y+j), img.at<float>(x,y)) && cmpf(val.at<float>(x+i,y+j), (val.at<float>(x,y) -1.0)) && lab.at<float>(x+i, y+j)>0 && lab.at<float>(x+i,y+j) < lmin){
                        lmin = lab.at<float>(x+i,y+j);
                    }
                }
            }
        }
    }
    
    for(int i=-1; i<=1; i++){
        for(int j=-1; j<=1; j++){
            if(!cmpf(lmin, LMAX) && !cmpf(lab.at<float>(x+i,y+j), lmin)){
                if(!cmpf(lab.at<float>(x,y), lmin)) changed = 1;
                lab.at<float>(x,y) = lmin;
            }
        }
    }
}


Mat Dispersao(Mat img, int filterSize, float threshold){
    Mat dispersaoImage = img.clone();
    float dispersao, sum = 0;
    float element, mean;
    for(int y=filterSize/2; y<dispersaoImage.rows-filterSize/2;y++)
    {
        for(int x = filterSize/2; x < dispersaoImage.cols-filterSize/2; x++)
        {
            element =0;
            sum =0;
            mean =0;
            for(int i=0; i<filterSize; i++){
                for(int j=0; j<filterSize; j++){
                    element = (float)img.at<uchar>(y - filterSize/2 + j ,x - filterSize/2 + i);
                    mean += element/(filterSize*filterSize);
                }
            }
            for(int i=0; i<filterSize; i++){
                for(int j=0; j<filterSize; j++){
                    element = (float)img.at<uchar>(y - filterSize/2 + j ,x - filterSize/2 + i);
                    sum +=abs(element-mean)/(filterSize*filterSize);
                    
                }
            }
            
            dispersao = sum;
            if(dispersao>threshold)    dispersaoImage.at<uchar>(y,x) = dispersao;
            else dispersaoImage.at<uchar>(y,x) = 255;
            
        }
    }
    return dispersaoImage;
}



Mat Watershed(Mat imagem){
    new_label = 0;
    scan_step2 = 1;
    scan_step3 = 1;
    
    img = imagem.clone();
    img.convertTo(img, CV_32F);
    VMAX = img.rows*img.cols;
    //inicializar o tamanho da matriz
    lab = img.clone();
    val = img.clone();
    
    //inicializar os valores
    for(int x = 0; x < img.rows; x++){
        for(int y = 0; y < img.cols; y++){
            lab.at<float>(x,y) = INIT;
            val.at<float>(x, y) = INIT;
        }
    }
    //encontra menor pixel de cada vizinhanca
    for(int x = 0; x < img.rows; x++){
        for(int y = 0; y < img.cols; y++){
            step1(x, y);
        }
    }
    
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
    
    //encontrar as labels
    while(scan_step3 == 1){
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
    
    float max=0;
    
    for(int x = 0; x < lab.rows; x++){
        for(int y = 0; y < lab.cols; y++){
            if(max < lab.at<float>(x,y)) max = lab.at<float>(x,y);
        }
    }
    
    lab.convertTo(lab, CV_8U,255.0/(max));
    
    return lab;
}
