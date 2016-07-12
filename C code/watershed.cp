
#include<iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

#include<queue>
#include<vector>
#include<algorithm>

#include <cstdlib>

using namespace std;
using namespace cv;

Mat lab, val, labV, imgFloat;
float max_pixel;
Mat img = imread("/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/lena.jpg", CV_LOAD_IMAGE_GRAYSCALE);
//Mat img = imread("/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/pb4.png", CV_LOAD_IMAGE_GRAYSCALE);

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
//
//Mat img = Mat(11, 10, CV_32F, data);

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
int curlab = 0;
int windowSize = 2;

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

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {

        cout<<"CLICK X: "<< x <<" Y: "<< y << " Valor: "<< labV.at<float>(x,y)<<" Orig: "<<imgFloat.at<float>(x,y)<<endl;
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
        cout<<"X: "<< x <<" Y: "<< y << " Valor: "<< labV.at<float>(y,x)<<endl;

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

int main(){
    Mat original = img.clone();
    img.convertTo(img, CV_32FC3);
    imgFloat = img.clone();
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
    //    int x=0;
    while(scan_step3 == 1){
        //        x++;
        //        if(x==9) break;
        changed = 0;
        //scan top left >> bottom right
        for(int x = 0; x < img.rows; x++){
            for(int y = 0; y < img.cols; y++){
                step3(x, y);
            }
        }
        //        cout<<lab;
        //        cin.get();

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



    labV = lab.clone();

    Mat color = color_watershed();
    //Cria janela
    namedWindow("original", 1);
    namedWindow("watershed", 2);

    //Cria callback
    setMouseCallback("original", CallBackFunc, NULL);
    setMouseCallback("watershed", CallBackFunc, NULL);

    lab.convertTo(lab, CV_8U,255.0/(max_pixel));
    img.convertTo(img, CV_8U);
    //Mostra imagem
//    imshow("original", original);
    imshow("color", color);
    //    waitKey(0);
    //    imshow("erode", img);
//    imshow("watershed", lab);
    waitKey(0);
}
