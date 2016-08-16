#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

Mat Dispersion(Mat img, int filterSize, float threshold){
    Mat dispersionImage = img.clone();
    float dispersion, sum = 0;
    float element, mean;
    for(int y=filterSize/2; y<dispersionImage.rows-filterSize/2;y++)
    {
        for(int x = filterSize/2; x < dispersionImage.cols-filterSize/2; x++)
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

            dispersion = sum;
            if(dispersion>threshold)    dispersionImage.at<uchar>(y,x) = dispersion;
            else dispersionImage.at<uchar>(y,x) = 255;

        }
    }
    return dispersionImage;
}

int main( int argc, char** argv )
{
    char *p=argv[0];
    int filterSize = 3, threshold= 7;
    string origin="", destination="";
    for(int i=0; i<argc; i++){
        p = argv[i];
        if(strcmp(p, "-t") == 0){
            threshold = atoi(argv[i+1]);
        }
        if(strcmp(p, "-f") == 0){
            filterSize = atoi(argv[i+1]);
        }
        if(strcmp(p, "-o") == 0){
            origin = argv[i+1];
        }
        if(strcmp(p, "-d") == 0){
            destination = argv[i+1];
        }
    }
    if(origin.compare("") == 0 || destination.compare("") == 0){
        cout<<"NO DESTINATION OR ORIGIN"<<endl;
        return -1;
    }

    Mat src = imread(origin);
    cvtColor(src, src, CV_BGR2GRAY);
    Mat result = Dispersion(src, filterSize, threshold);

    imwrite(destination,result);
    return 0;
}
