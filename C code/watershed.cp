
#include<iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

#include<queue>
#include<vector>
#include<algorithm>

using namespace std;
using namespace cv;

Mat lab, val;
Mat img = imread("lena.jpg", CV_LOAD_IMAGE_GRAYSCALE);

//float data[11][10] =
//{{3,5,5,2,8,8,8,11,10,10},
//    {5,5,11,11,8,11,11,8,10,10},
//    {11,5,11,11,9,9,9,9,8,10},
//    {11,11,11,7,7,7,7,9,9,8},
//    {11,11,11,11,11,9,7,10,8,10},
//    {11,10,11,9,7,7,9,9,10,8},
//    {11,10,11,9,11,9,10,10,8,10},
//    {11,11,11,8,8,8,8,8,10,10},
//    {11,11,11,11,10,10,10,10,10,10},
//    {10,10,10,10,10,10,10,10,10,10},
//    {11,11,11,11,10,10,10,10,10,10}};

//Mat img = Mat(11, 10, CV_32F, data);

const float INIT = 0;
int changed = 0;
float new_label = 0;
int scan_step2 = 1;
int scan_step3 = 1;
float VMAX, LMAX;
int curlab = 0;

void step1(int x, int y){
    if(val.at<float>(x,y) != 1){
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
                if(val.at<float>(x,y) != 1.0){
                    if(img.at<float>(x,y) > img.at<float>(x+i,y+j)) val.at<float>(x,y) = 1.0;
                }
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
                
                //novo valor do pixel baseado nos minimos vizinhos
                if(min != VMAX && val.at<float>(x,y) != min + 1 && val.at<float>(x,y) < min && val.at<float>(x,y)==0){
                    val.at<float>(x,y) = min+1;
                    LMAX = min+1;
                    changed = 1;
                }
            }
        }
    }
}

void step3(int x, int y){
    float lmin = LMAX;
    float fmin = img.at<float>(x, y);
    if(val.at<float>(x,y) == 0){
        
        for(int i=-1; i<=1; i++){
            for(int j=-1; j<=1; j++){
                //Casos de borda
                if(x == 0 && i < 0) continue;
                if(y == 0 && j < 0) continue;
                if(x == img.rows-1  && i > 0) continue;
                if(y == img.cols-1 && j > 0) continue;
                //nao pegar o pixel no meio
                if(i == 0 && j == 0) continue;
                
                if(img.at<float>(x,y) == img.at<float>(x+i,y+j) && lab.at<float>(x+i,y+j)>0 && lab.at<float>(x+i,y+j) < lmin){
                    lmin = lab.at<float>(x+i,y+j);
                }
            }
        }
        if(lmin == LMAX && lab.at<float>(x,y) == 0){
            lmin = ++new_label;
        }
        
    }else{
        if(val.at<float>(x,y) == 1){
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
                    
                    if(img.at<float>(x+i,y+j) == fmin && lab.at<float>(x+i,y+j) > 0 && lab.at<float>(x+i,y+j) < lmin){
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
                    
                    if(img.at<float>(x+i,y+j) == img.at<float>(x,y) && (val.at<float>(x+i,y+j) == (val.at<float>(x,y) -1)) && lab.at<float>(x+i, y+j)>0 && lab.at<float>(x+i,y+j) < lmin){
                        lmin = lab.at<float>(x+i,y+j);
                    }
                }
            }
        }
    }
    
    if(lmin != LMAX && lab.at<float>(x,y) != lmin){
        lab.at<float>(x,y) = lmin;
        changed = 1;
    }
}

int main(){
    Mat original = img.clone();
    img.convertTo(img, CV_32FC1);
    VMAX = img.rows*img.cols;
    //inicializar o tamanho da matriz
    lab = img.clone();
    val = img.clone();
    vector<uchar> grayScaleSortedLevel;
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
    
    //    cout<<val;
    
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
    
    //    cout<<val<<endl;
    //    cout<<LMAX;
    
    //encontrar as labels
    while(scan_step3 == 1){
        changed = 0;
        //scan top left >> bottom right
        for(int x = 0; x < img.rows; x++){
            for(int y = 0; y < img.cols; y++){
                step3(x, y);
            }
        }
        cout<<lab;
        cin.get();
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
    
    
    
    //    imwrite("final.jpg", lab);
    //    for(int x = 0; x < lab.rows; x++){
    //        for(int y = 0; y < lab.cols; y++){
    //            if(lab.at<float>(x,y) == 0){
    //                lab.at<float>(x,y) = img.at<float>(x,y);
    //            }
    //            else {
    //                lab.at<float>(x,y) = 0;
    //            }
    //        }
    //    }
    imshow("original", original);
    imshow("watershed", lab);
    
    cin.get();
}
