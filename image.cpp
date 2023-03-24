#include <iostream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <cmath>
#include <string>
#include "image.h"


Image::Image(std::string path, cv::Mat data)
: _path(path)
, _data(data)
{
}

void Image::show(cv::Mat sourceImg) const{
    cv::imshow("Source", sourceImg);
}

void Image::showResult(cv::Mat filterImg) const{
    cv::imshow("Result", filterImg);
    cv::waitKey(0);
}