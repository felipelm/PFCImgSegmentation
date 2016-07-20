#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>

using namespace cv;
using namespace std;

Mat src; Mat src_gray;



class Region{
private:
    uchar label;
    int numOfPixels;
    map<uchar, int> neighbor;

public:
    Region(uchar label){
        this->label = label;
        this->numOfPixels = 1;
    }

    Region(){

    }

    void addNeighbor(uchar neighborLabel){
        if(neighbor.find(neighborLabel) != neighbor.end()){
            //nova regiao
            neighbor[neighborLabel] = 1;
        }else{
            neighbor[neighborLabel]++;
        }
    }

    int getLabel(){
        return this->label;
    }

    int getNumOfPixels(){
        return this->numOfPixels;
    }

    void addPixel(){
        this->numOfPixels++;
    }

    void print(){
        for(map<uchar, int>::iterator it = neighbor.begin();
            it != neighbor.end(); it++)
        {
            std::cout << it->first << " " << it->second;
        }
    }
};

void mergeRegion(Mat labImg, int threshold){
    map<uchar, Region> regions;
    for(int y=0; y<labImg.rows; y++){
        for(int x=0; x<labImg.cols; x++){
            if(regions.find(labImg.at<uchar>(y,x)) != regions.end()){
                //nova regiao
                regions[labImg.at<uchar>(y,x)] = *new Region(labImg.at<uchar>(y,x));
            }else{
                //mais um de uma regiao
                regions[labImg.at<uchar>(y,x)].addPixel();
            }
            for(int i=-1; i<=1; i++){
                for(int j=-1; j<=1; j++){
                    //Casos de borda
                    if(!(x + i < 0) &&
                       !(y + j < 0) &&
                       !(x+i > labImg.rows-1) &&
                       !(y+j > labImg.cols-1) &&
                       //nao pegar o pixel no meio
                       !(i == 0 && j == 0)){
                        //add vizinho
                        regions[labImg.at<uchar>(y,x)].addNeighbor(labImg.at<uchar>(y,x));
                    }
                }
            }
        }
    }
    int cnt = 0;
    for(map<uchar, Region>::iterator it = regions.begin();
        it != regions.end(); it++)
        cnt++;
      cout<<"cnt: "<<cnt<<endl;

}

/** @function main */
int main( int argc, char** argv )
{
    /// Load source image and convert it to gray
    src = imread( "watershedGreyscale.png", 1 );

    /// Convert image to gray and blur it
    cvtColor( src, src_gray, CV_BGR2GRAY );
//    blur( src_gray, src_gray, Size(3,3) );

    src_gray.convertTo(src_gray, CV_8U);
    mergeRegion(src_gray, 10);


    /// Create Window
    String source_window = "Source";
    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    imshow( source_window, src );

    namedWindow( "grey", CV_WINDOW_AUTOSIZE );
    imshow( "grey", src_gray );


    waitKey(0);
    return(0);
}
