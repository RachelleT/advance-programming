#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <opencv2/opencv.hpp>

class Image {
    protected:
        std::string _path;
        cv::Mat _data;
        int chs;

    public:
        Image(std::string path, cv::Mat data);
        void show(cv::Mat sourceImg) const;
        void showResult(cv::Mat filterImg) const;
};

#endif // IMAGE_Η