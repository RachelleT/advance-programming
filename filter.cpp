#include <iostream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include <cmath>
#include <string>

#include "filter.h"


Filter::Filter(int type, int size)
: _type(type)
, _size(size)
, _kernel()
{ 
  Filter::createKernel();
}

void Filter::createKernel(){
  cv::Mat lowpass;
  cv::Mat highpass;
  double sigma = 1.0;
  double sum = 0.0;

  auto size_sq = _size*_size;

  switch(_type)
  {
  case 1: //Low pass
    _kernel = cv::Mat(size_sq, 1, CV_64F, 1.0 / (size_sq));

    std::cout << "Matrix: " ;
    for (int i =0; i < _kernel.rows; i++){
      std::cout << _kernel.at<double>(i) << ' ';
    }

    break;

  case 2: //High Pass
    _kernel = cv::Mat(size_sq, 1, CV_64F, cv::Scalar(-1, -1, -1));
    _kernel.at<double>(size_sq/2) = (size_sq) - 1;

    std::cout << "Matrix: " ;
    for (int i =0; i < _kernel.rows; i++){
      std::cout << _kernel.at<double>(i) << ' ';
    }

    break;

  case 3: //Band Pass
    _kernel = cv::Mat(_size, _size, CV_64F); //allocate previously
    lowpass = cv::Mat(_size, _size , CV_64F, 1.0/(size_sq));
    highpass = cv::Mat(_size, _size, CV_64F, cv::Scalar(-1, -1, -1));
    highpass.at<double>(_size/2, _size/2) = (size_sq) - 1;

    std::transform(lowpass.begin<double>(), lowpass.end<double>(), highpass.begin<double>(), _kernel.begin<double>(), std::multiplies<double>() );

    _kernel = _kernel.reshape(size_sq);
    std::cout << "Matrix: \n" << _kernel << std::endl;
    break;

  case 4: //Gaussian Filter
    _kernel = cv::Mat(size_sq, 1, CV_64F); //allocate previously 
    for (int i = 0; i < _size; i++)
    {
      for (int j = 0; j < _size; j++)
      {
        _kernel.at<double>(i+j*_size) = exp(-((i - _size / 2) * (i - _size / 2) + (j - _size / 2) * (j - _size / 2)) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
        sum += _kernel.at<double>(i + j * _size);
      }
    }

    //normalize
    for (int i = 0; i < _size; i++)
    {
      for (int j = 0; j < _size; j++)
      {
        _kernel.at<double>(i + j * _size) /= sum;
      }
    }

    std::cout << "Matrix: \n" << _kernel << std::endl;
    break;

  case 5: //Laplacian Filter
    _kernel = cv::Mat(size_sq, 1, CV_64F); //allocate previously 
    for (int i = 0; i < _size; i++)
    {
      for (int j = 0; j < _size; j++)
      {
        _kernel.at<double>(i + j * _size) = (exp(-((i - _size / 2) * (i - _size / 2) + (j - _size / 2) * (j - _size / 2)) / (2 * sigma * sigma))) * (1 - (((i - _size / 2) * (i - _size / 2) + (j - _size / 2) * (j - _size / 2)) / (2 * sigma * sigma))) / ((M_PI * sigma * sigma * sigma * sigma));
        sum += _kernel.at<double>(i + j * _size);

      }
    }

    //normalize
    for (int i = 0; i < _size; i++)
    {
      for (int j = 0; j < _size; j++)
      {
        _kernel.at<double>(i + j * _size) /= sum;
      }
    }

    std::cout << "Matrix: \n" << _kernel << std::endl;
    break;

  default:
    std::cout << "Invalid option selected." << std::endl;
    break;
  }
}


cv::Mat Filter::applyfilter(cv::Mat image, int chs)
{
  int pad = _size / 2;

  cv::Mat img = image.reshape(1, 0);
  Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> mat;

  int newXSize = image.size().height + _size;
  int newYSize = image.size().width + _size;
  cv::Mat padimg(newXSize* newYSize, 1, CV_64FC3);

  std::unique_ptr<cv::Mat> filt_img_ptr = std::make_unique<cv::Mat>(cv::Mat(image.size(), CV_8UC3));

  cv::cv2eigen(img, mat);
  
  cv::Mat img2;

  cv::eigen2cv(mat, img2);
  img2 = img2.reshape(chs, 0);

  //Zero-padding
  for (int y = 0; y < img2.rows - 1; y++)
  {
    for (int x = 0; x < img2.cols - 1; x++)
    {
      padimg.at<cv::Vec3d>(x + pad + (y + pad) * newYSize) = img2.at<cv::Vec3b>(cv::Point(x, y)); //implicit cast from uchar to double
    }
  }

  cv::Vec3d color;
  for (int y = 0; y < newXSize - _size; y++)
  {
    for (int x = 0; x < newYSize - _size; x++)
    {
      for (int yy = 0; yy < _size; yy++)
      {
        for (int xx = 0; xx < _size; xx++)
        {
          color = color + padimg.at<cv::Vec3d>(x + xx + (y + yy) * newYSize) * _kernel.at<double>(xx + yy * _size); //operation between doubles
        }
      }
      filt_img_ptr->at<cv::Vec3b>(cv::Point(x, y)) = color; //implicit cast from double to uchar

      /*
        This cast can be performed as we have previously normalize the filter and the values of the resulting pixel will be 
        between 0-255 (uchar), even though the partial sum of operation in a convolution can be outside of this range.
        This is the reason for having a temp variable (color) of type double and avoid losing precision in this step.
        */

      color = 0.0;
    }
  }
  return *filt_img_ptr;
}