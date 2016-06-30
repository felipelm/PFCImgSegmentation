#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

Mat Variance(Mat img, int filterSize, float threshold){
    Mat varianceImage = img.clone();
    float variance, sum = 0;
    float element, mean;
    //            for(int y = 0; y < img.rows; y++)
    //                for(int x = 0; x < img.cols; x++)
    //                    varianceImage.at<uchar>(y,x) = 0.0;
    for(int y=filterSize/2; y<varianceImage.rows-filterSize/2;y++)
    {
        for(int x = filterSize/2; x < varianceImage.cols-filterSize/2; x++)
        {
            element =0;
            sum =0;
            mean =0;
            for(int i=0; i<filterSize; i++){
                for(int j=0; j<filterSize; j++){
                    element = (float)img.at<uchar>(y - filterSize/2 + j ,x - filterSize/2 + i);
                    mean += element/(filterSize*filterSize);
                    sum +=element*element/(filterSize*filterSize);
                    
                }
            }
            variance = sum - mean*mean;
            //                    cout<<variance<<endl;
            if(variance>threshold)    varianceImage.at<uchar>(y,x) = variance;
            
        }
    }
    return varianceImage;
}

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

Mat MedianFilter(Mat img, unsigned int filterSize){
    Mat imageWithMedianFilter = img.clone();
    int window[filterSize*filterSize];
    for(int y = 0; y < img.rows; y++)
        for(int x = 0; x < img.cols; x++)
            imageWithMedianFilter.at<uchar>(y,x) = 0.0;
    for(int y=filterSize/2; y<imageWithMedianFilter.rows-filterSize/2;y++)
    {
        for(int x = filterSize/2; x < imageWithMedianFilter.cols-filterSize/2; x++)
        {
            for(int i=0; i<filterSize; i++){
                for(int j=0; j<filterSize; j++){
                    window[i*filterSize+j]= img.at<uchar>(y - filterSize/2 + j ,x - filterSize/2 + i);
                }
            }
            //sort the window to find median FILTRO
            insertionSort(window);
            
            //assign the median to centered element of the matrix FILTRO
            imageWithMedianFilter.at<uchar>(y,x) = window[filterSize/2];
        }
    }
    return imageWithMedianFilter;
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

Mat watershed_filter(Mat src, Mat dst1){
    //transformar imagem pro formato correto
//    cvtColor(dst1, dst1, CV_BGR2GRAY);
//    Mudar o fundo de branco pra preto para melhorar os resultados do Distance transform
//        for( int x = 0; x < src.rows; x++ ) {
//            for( int y = 0; y < src.cols; y++ ) {
//                if ( src.at<Vec3b>(x, y) == Vec3b(255,255,255) ) {
//                    src.at<Vec3b>(x, y)[0] = 0;
//                    src.at<Vec3b>(x, y)[1] = 0;
//                    src.at<Vec3b>(x, y)[2] = 0;
//                }
//            }
//        }

//        imshow("Fundo preto", src);
    
    // kernel pro sharpening
    Mat kernel = (Mat_<float>(3,3) <<
                  1,  1, 1,
                  1, -8, 1,
                  1,  1, 1); // aproximacao da segunda derivada

    // aplicar sharp, laplaciano e truncar numero negativos convertendo pra 0...255 com CV_8U
    Mat imgLaplacian;
    Mat sharp = src; // copy source image to another temporary one
    filter2D(sharp, imgLaplacian, CV_32F, kernel);
    src.convertTo(sharp, CV_32F);
    Mat imgResult = sharp - imgLaplacian;
    // convert back to 8bits gray scale
    imgResult.convertTo(imgResult, CV_8UC3);
    imgLaplacian.convertTo(imgLaplacian, CV_8UC3);
    
//    imshow( "Laplaciano", imgLaplacian );
//    imshow( "Sharpen", imgResult );
    
    src = imgResult; // copy back
    // Imagem binaria da imagem original
    Mat bw=dst1.clone();
    
    
//        cvtColor(src, bw, CV_BGR2GRAY);
    
    
    threshold(bw, bw, 40, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    
//        imshow("Binary Image", bw);
    
    
    // Perform the distance transform algorithm
    Mat dist;
    distanceTransform(bw, dist, CV_DIST_L2, 3);
    // Normalize the distance image for range = {0.0, 1.0}
    // so we can visualize and threshold it
    normalize(dist, dist, 0, 10., NORM_MINMAX);
    
//        imshow("Distance Transform", dist);
    
    // Threshold para obter os vales
    // Usado nos marcadores dependendo do que quer que seja vale: CV_THRESH_BINARY THRESH_BINARY_INV
    threshold(dist, dist, .4, 1., THRESH_BINARY_INV);
    // Dilatar a imagem
    Mat kernel1 = Mat::ones(3, 3, CV_8UC1);
    
//    imshow("antes erode", dist);
    
    dilate(dist, dist, kernel1);
    //Erodir a imagem
    erode(dist, dist, kernel1, Point(-1, -1), 10);
    
//    imshow("vales", dist);
    
    // Cria versao CV_8U da imagem de distancia
    // usado para findContours()
    Mat dist_8u;
    dist.convertTo(dist_8u, CV_8U);
    
   
    
    // Encontrar os marcadores
    vector<vector<Point> > contours;
    findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    // Cria imagem usada para realizar o watershed
    Mat markers = Mat::zeros(dist.size(), CV_32SC1);
    // Desenha oq esta em foreground
    for (size_t i = 0; i < contours.size(); i++)
        drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i)+1), -1);
    // Desenha oq esta em background
    circle(markers, Point(5,5), 1, CV_RGB(255,255,255), -1);
    
//    imshow("Marcadores", markers*10000);
    
    // Realiza watershed
    watershed(src, markers);
    Mat mark = Mat::zeros(markers.size(), CV_8UC1);
    markers.convertTo(mark, CV_8UC1);
    bitwise_not(mark, mark);
    
//        imshow("Marcadores_v2", mark); // ver marcadores
    

    // Gera cores aleatorias
    vector<Vec3b> colors;
    for (size_t i = 0; i < contours.size(); i++)
    {
        int b = theRNG().uniform(0, 255);
        int g = theRNG().uniform(0, 255);
        int r = theRNG().uniform(0, 255);
        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }
    // Cria imagem final
    Mat dst = Mat::zeros(markers.size(), CV_8UC3);
    // Pinta cada area de uma cor
    for (int i = 0; i < markers.rows; i++)
    {
        for (int j = 0; j < markers.cols; j++)
        {
            int index = markers.at<int>(i,j);
            if (index > 0 && index <= static_cast<int>(contours.size()))
                dst.at<Vec3b>(i,j) = colors[index-1];
            else
                dst.at<Vec3b>(i,j) = Vec3b(0,0,0);
        }
    }
    return dst;
    
}

Mat mapaBordas(Mat img, int filterSize, float threshold){
    Mat auxImg = img.clone();
    int altaFreq = 0;
    for(int y=filterSize/2; y<auxImg.rows-filterSize/2;y++)
    {
        for(int x = filterSize/2; x < auxImg.cols-filterSize/2; x++)
        {
            altaFreq =0;
            for(int i=0; i<filterSize; i++){
                for(int j=0; j<filterSize; j++){
//                    cout<<(float) auxImg.at<uchar>(i,j)<<" ";

                    if( (float) auxImg.at<uchar>(i,j) > 170){
                        altaFreq++;
                    }
                }
            }
//            if(altaFreq !=29 ) cout<<altaFreq<<" ";
            if(altaFreq>threshold) {
                img.at<uchar>(y,x) = 255;
                
            }
        }
    }
    return img;
}

Mat src, srcAux; Mat dst1;
int thresh_dispersao = 20, janela_dispersao = 3, max_janela_disp = 21;
int thresh_variancia = 7, janela_variancia = 3, max_janela_var = 21;
int janela_mediana=3, max_janela_med = 21;

int max_thresh = 255;
RNG rng(12345);

void thresh_callback(int, void* );

int main(int, char** argv)
{
//    Mat src, dst1,srcAux, dst, dst2;
    src = imread("/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/lena.jpg");
    srcAux = src.clone();
    cvtColor(srcAux, srcAux, CV_BGR2GRAY);
//    dst1 = MedianFilter(srcAux, 3);
//    dst1 = Dispersao(dst1, 3, 7);
//    dst1 = mapaBordas(dst1, 15, 225);
    

//    dst1= Variance(dst1, 3, 20);
    
    // Load the image
    // Check if everything was fine
    if (!src.data)
        return -1;
    // Show source image
//    imshow("Original", src);
//    imshow("dispersao", dst1);
//    imshow("variancia", dst2);
    
    //teste marcando moedas na mao
//    dst1 = imread("/Users/felipemachado/Dropbox/Estudo/PFC/imagensPFC/testeWatershedmarkers.jpg");
//    cvtColor(src, src, CV_BGR2GRAY);
    
//    dst = watershed_filter(src, dst1);
//    imshow("Imagem Segmentada", dst);
    /// Create Window
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
    
    

    waitKey(0);
    return 0;
}

void thresh_callback(int, void* )
{
    Mat output, dispersao_show, variancia_show, mediana_show;
    
    mediana_show = MedianFilter(srcAux, janela_mediana);
    dispersao_show = Dispersao(mediana_show, janela_dispersao, thresh_dispersao);
//    variancia_show = Dispersao(mediana_show, janela_variancia, thresh_variancia);
    output = watershed_filter(src, dispersao_show);
    cout<<"ThreshDisp: "<<thresh_dispersao<<" JanDisp: "<<janela_dispersao<<" JanMed: "<<janela_mediana<<endl;
    
//    namedWindow( "Variancia", CV_WINDOW_AUTOSIZE ); imshow( "Variancia", variancia_show );
    namedWindow( "Dispersao", CV_WINDOW_AUTOSIZE ); imshow( "Dispersao", dispersao_show );
    namedWindow( "Resultado", CV_WINDOW_AUTOSIZE ); imshow( "Resultado", output );
}