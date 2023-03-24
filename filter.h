#ifndef FILTER_H
#define FILTER_H

#include <string>
#include <opencv2/opencv.hpp>

class Filter {
    protected:
        int _type;
        int _size;
        cv::Mat _kernel;

    public:
        Filter(int type, int size);
        void createKernel();
        cv::Mat applyfilter(cv::Mat image, int chs);
};

#endif // FILTER_Η