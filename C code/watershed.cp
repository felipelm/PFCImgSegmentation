
#include<iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

#include<queue>
#include<vector>
#include<algorithm>

using namespace std;
using namespace cv;



Mat lab, val, colored;
float max_pixel;
Mat img = imread("lena.jpg", CV_LOAD_IMAGE_GRAYSCALE);
//Mat img = imread("/Users/felipemachado/Documents/OpenCV/workspace/bob-esponja.jpg", CV_LOAD_IMAGE_GRAYSCALE);
//Mat img = imread("/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/chessboard.png", CV_LOAD_IMAGE_GRAYSCALE);
//Mat img = imread("/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/cards.png", CV_LOAD_IMAGE_GRAYSCALE);

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
int curlab = 0;
int windowSize = 20;

void step1(int x, int y){
    if(val.at<float>(x,y) != 1.0){
        //inspecionar os vizinhos do pixel para setar o marcador
        for(int i=-windowSize; i<=windowSize; i++){
            for(int j=-windowSize; j<=windowSize; j++){
                //Casos de borda
                if(x+i < 0) continue;
                if(y+j < 0) continue;
                if(x+i > img.rows-1) continue;
                if(y+j > img.cols-1) continue;
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

        for(int i=-windowSize; i<=windowSize; i++){
            for(int j=-windowSize; j<=windowSize; j++){
                //Casos de borda
                if(x+i < 0) continue;
                if(y+j < 0) continue;
                if(x+i > img.rows-1) continue;
                if(y+j > img.cols-1) continue;
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

      for(int i=-windowSize; i<=windowSize; i++){
          for(int j=-windowSize; j<=windowSize; j++){
              //Casos de borda
              if(x+i < 0) continue;
              if(y+j < 0) continue;
              if(x+i > img.rows-1) continue;
              if(y+j > img.cols-1) continue;
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
          for(int i=-windowSize; i<=windowSize; i++){
              for(int j=-windowSize; j<=windowSize; j++){
                  //Casos de borda
                  if(x+i < 0) continue;
                  if(y+j < 0) continue;
                  if(x+i > img.rows-1) continue;
                  if(y+j > img.cols-1) continue;
                    //nao pegar o pixel no meio
                    if(i == 0 && j == 0) continue;

                    if(img.at<float>(x+i,y+j)<fmin){
                        fmin = img.at<float>(x+i,y+j);
                    }
                }
            }

            for(int i=-windowSize; i<=windowSize; i++){
                for(int j=-windowSize; j<=windowSize; j++){
                    //Casos de borda
                    if(x+i < 0) continue;
                    if(y+j < 0) continue;
                    if(x+i > img.rows-1) continue;
                    if(y+j > img.cols-1) continue;
                    //nao pegar o pixel no meio
                    if(i == 0 && j == 0) continue;

                    if(cmpf(img.at<float>(x+i,y+j), fmin) && lab.at<float>(x+i,y+j) > 0.0 && lab.at<float>(x+i,y+j) < lmin){
                        lmin = lab.at<float>(x+i,y+j);
                    }
                }
            }
        }else{
            for(int i=-windowSize; i<=windowSize; i++){
                for(int j=-windowSize; j<=windowSize; j++){
                    //Casos de borda
                    if(x+i < 0) continue;
                    if(y+j < 0) continue;
                    if(x+i > img.rows-1) continue;
                    if(y+j > img.cols-1) continue;
                    //nao pegar o pixel no meio
                    if(i == 0 && j == 0) continue;

                    if(cmpf(img.at<float>(x+i,y+j), img.at<float>(x,y)) && cmpf(val.at<float>(x+i,y+j), (val.at<float>(x,y) -1.0)) && lab.at<float>(x+i, y+j)>0 && lab.at<float>(x+i,y+j) < lmin){
                        lmin = lab.at<float>(x+i,y+j);
                    }
                }
            }
        }
    }

    for(int i=-windowSize; i<=windowSize; i++){
        for(int j=-windowSize; j<=windowSize; j++){
          if(x+i < 0) continue;
          if(y+j < 0) continue;
          if(x+i > img.rows-1) continue;
          if(y+j > img.cols-1) continue;
            //nao pegar o pixel no meio
            if(i == 0 && j == 0) continue;

            if(!cmpf(lmin, LMAX) && !cmpf(lab.at<float>(x+i,y+j), lmin)){
                if(!cmpf(lab.at<float>(x,y), lmin)) changed = 1;
                lab.at<float>(x,y) = lmin;
            }
        }
    }
    //    cout<<lab;
    //    cin.get();
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

void color_regions(){
    // Gera cores aleatorias
    vector<Vec3b> colors;
    for (size_t i = 1; i <= max_pixel; i++)
    {
        //        int b = theRNG().uniform(1, 255);
        //        int g = theRNG().uniform(1, 255);
        //        int r = theRNG().uniform(1, 255);
        int b = rand() % 255+1;
        int g = rand() % 255+1;
        int r = rand() % 255+1;
        colors.push_back(Vec3b(b, g, r));
        //        cout<<b<<" "<<g<<" "<<r<<endl<<Vec3b(b, g, r);
        ////        cout<<(uchar)b<<" "<<(uchar)g<<" "<<(uchar)r<<endl;
        //        cout<<colors[i-1];
        //        cin.get();
    }

    // Pinta cada area de uma cor
    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            int index = ((int) lab.at<float>(i,j)) % 255;
            //            cout<<index<<" "<<colors[index-1];
            //            cin.get();
            if (index > 0)
                colored.at<Vec3b>(i,j) = colors[index-1];
            else
                colored.at<Vec3b>(i,j) = Vec3b(0,0,0);
        }
    }
}

void initialize_matrix(){
    //inicializar o tamanho da matriz
    lab = img.clone();
    val = img.clone();
    colored = img.clone();
    colored.convertTo(colored, CV_64F);

    //inicializar os valores
    for(int x = 0; x < img.rows; x++){
        for(int y = 0; y < img.cols; y++){
            lab.at<float>(x,y) = INIT;
            val.at<float>(x, y) = INIT;
            colored.at<Vec3b>(x, y) = Vec3b(0,0,0);
        }
    }
    //    cout<<colored.at<Vec3b>(0, 0);
    //    cin.get();
}

int main(){
    Mat original = img.clone();
    //    img = Dispersao(img, 7, 2.0);
    img.convertTo(img, CV_32F);
    VMAX = img.rows*img.cols;
    //inicializar o tamanho da matriz
    initialize_matrix();

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
              //  cout<<"a";
              //  cin.get();

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
              //  cout<<"a";
              //  cin.get();
    }

    //    cout<<lab;

    max_pixel=0;

    for(int x = 0; x < lab.rows; x++){
        for(int y = 0; y < lab.cols; y++){
            if(max_pixel < lab.at<float>(x,y)) max_pixel = lab.at<float>(x,y);
        }
    }

    // lab.convertTo(lab, CV_8U);

//    color_regions();

    cout<<lab;
    cin.get();
    lab.convertTo(lab, CV_8U,255.0/(max_pixel));
    img.convertTo(img, CV_8U);
    //    colored.convertTo(colored, CV_8U);


    //    cout<<colored.at<Vec3b>(1, 0)<<colored.at<uchar>(1, 0)<<"teste";
    //    cin.get();
    //        imwrite("colored.jpg", colored);
    cout<<"a";
    imwrite("original.jpg", original);
    cout<<lab;
    //    imshow("dispersao", img);
    imwrite("watershed.jpg", lab);
    cout<<"c";
    cin.get();
    imwrite("color.jpg", colored);
    cout<<"d";

    cin.get();
}

