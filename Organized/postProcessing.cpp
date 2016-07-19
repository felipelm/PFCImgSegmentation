class Region{
  private:
    int label;
    int numOfPixels;
    vector<int> *neighbor;

  public:
    Region(int label, int numberOfPixels){
      this->label = label;
      this->numberOfPixels = numberOfPixels;
      this->neighbor = new vector<int>();
    }

    void addNeighbor(int neighborLabel){
      (this->neighbor)->push_back(neighbor);
    }

    int getLabel(){
      return this->label;
    }

    int getNumOfPixels(){
      return this->numOfPixels;
    }
}

Mat mergeRegion(Mat labImg, int threshold){
  map<uchar, int> regions;
  for(int y=0; y<labImg.rows; y++){
    for(int x=0; x<labImg.cols; x++){
      if(regions.find(labImg.at<uchar>(y,x))){
        map[labImg.at<uchar>(y,x)] = 0;
      }
      map[labImg.at<uchar>(y,x)]++;
    }
  }
  vector<Region> regions;
  for(map::iterator it= map.begin(); it != map.end(); it++){
    regions.push_back(new Region(it->first, it->second));
  }
  for(int)

}
