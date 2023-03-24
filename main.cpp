#include <iostream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <string>

#include "filter.h"
#include "image.h"


int main(int argc, char **argv)
{
  std::string imgName;

  std::cout << "Please enter the path of your image (with the extension): ";
  std::cin >> imgName;

  //Create image object
  cv::Mat image = cv::imread(imgName);
  if (!image.data)
  {
    std::cout << "No image data \n" ;
    return EXIT_FAILURE;
  }

  Image sourceImage(imgName, image);
  sourceImage.show(image);
  int chs = image.channels();

  //Choose the filter to apply
  int filterChosen;
  std::cout << "Please select the filter to be applied (number):\n\t1. Low pass \n\t2. High pass \n\t3. Bandpass filter \n\t4. Gaussian Filter \n\t5. Laplacian Filter \n\t";
  std::cin >> filterChosen;

  //Choose the filter size
  int rad = 0;
  int filtSize = 0;
  while (rad * 2 == filtSize){
    std::cout << "Please enter an odd filter size (e.g 3 for (3 x 3)): ";
    std::cin >> filtSize;

    rad = filtSize / 2; //implicit cast to int
  }

  //Filtering
  Filter filt(filterChosen, filtSize);
  cv::Mat img = filt.applyfilter(image, chs);
        
  //Visualization
  Image filtImg(imgName, img);
  filtImg.showResult(img);
}
