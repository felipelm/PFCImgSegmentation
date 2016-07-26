//
//  mergeRegion.cpp
//  openCV Pacote
//
//  Created by Felipe Machado on 20/07/16.
//  Copyright © 2016 Felipe Machado. All rights reserved.
//

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "handler.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>

using namespace cv;
using namespace std;

Mat similarityMatrix;

class Region{
private:
    uchar label;
    int numOfPixels;
    double similaritySum;

public:
    map<uchar, int> neighbor;
    Region(uchar label, uchar similarityPixel){
        this->label = label;
        this->numOfPixels = 1;
        this->similaritySum = similarityPixel;
    }

    Region(){

    }

    void addNeighbor(uchar neighborLabel){
        if(neighbor.find(neighborLabel) == neighbor.end()){
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

    map<uchar, int> getNeighbor(){
        return neighbor;
    }

    double getSimilaritySum(){
        return similaritySum;
    }

    void addPixel(){
        (this->numOfPixels)+=1;
    }

    void addSimilarityPixel(uchar similarityPixel){
        similaritySum += similarityPixel;
    }

    void print(){
        for(map<uchar, int>::iterator it = neighbor.begin();
            it != neighbor.end(); it++)
        {
            std::cout << it->first << " " << it->second;
        }
    }
};

map<uchar, Region> regions;

uchar getMergeNeighbor(Region currentRegion){
    double bestSimilarity = 1000000000;
    uchar bestRegion = 0;
    int currentArea = currentRegion.getNumOfPixels();
    int currentSum = currentRegion.getSimilaritySum();
    double similarity = bestSimilarity;
    for(map<uchar, int>::iterator it = (currentRegion.neighbor).begin();
        it != (currentRegion.neighbor).end();
        it++){

        int neighborArea = regions[it->first].getNumOfPixels();
        int neighborSum = regions[it->first].getSimilaritySum();
        if(currentArea != 0 && neighborArea !=0){
            similarity = currentArea*neighborArea/(currentArea+neighborArea)*abs(currentSum/currentArea - neighborSum/neighborArea);
        }


        if(similarity < bestSimilarity){
            bestSimilarity = similarity;
            bestRegion = it->first;
        }

    }
    return bestRegion;
}

Mat merge(Mat img, uchar original, uchar newValue){
    //scan top left >> bottom right
    for(int x = 0; x < img.rows; x++){
        for(int y = 0; y < img.cols; y++){
            if(img.at<uchar>(x,y) == original){
                img.at<uchar>(x,y) = newValue;
            }
        }
    }
    return img;
}

Mat mergeRegion(Mat labImg, int threshold, Mat dispersion){
    similarityMatrix = dispersion;
    for(int y=0; y<labImg.rows; y++){
        for(int x=0; x<labImg.cols; x++){
            if(regions.find(labImg.at<uchar>(y,x)) == regions.end()){
                //nova regiao
                regions[labImg.at<uchar>(y,x)] = *new Region(labImg.at<uchar>(y,x), similarityMatrix.at<uchar>(y,x));
            }else{
                //mais um de uma regiao
                regions[labImg.at<uchar>(y,x)].addPixel();
                regions[labImg.at<uchar>(y,x)].addSimilarityPixel(similarityMatrix.at<uchar>(y,x));
            }

            for(int i=-1; i<=1; i++){
                for(int j=-1; j<=1; j++){
                    //Casos de borda
                    if(!(x + i < 0) &&
                       !(y + j < 0) &&
                       !(x+i > labImg.rows-1) &&
                       !(y+j > labImg.cols-1) &&
                       //nao pegar o pixel no meio
                       !(i == 0 && j == 0) &&
                       (labImg.at<uchar>(y+j,x+i) > (uchar) 15)){
                        //add vizinho
                        regions[labImg.at<uchar>(y,x)].addNeighbor(labImg.at<uchar>(y+j,x+i));
                    }
                }
            }
        }
    }
    Mat aux = labImg.clone();
    int cnt = 0, merges=0;
    for(map<uchar, Region>::iterator it = regions.begin();
        it != regions.end(); it++){
        cout<<(it->first)<<" "<<(it->second).getNumOfPixels()<<endl;
        cnt++;
        uchar mergeRegion = getMergeNeighbor((it->second));
        if((it->second).getNumOfPixels() < threshold){
            merges++;
            aux = merge(aux, (it->first), mergeRegion);
        }
    }
    cout<<merges<<endl;
    regions.clear();
    return aux;

}
