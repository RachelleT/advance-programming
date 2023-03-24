# Image filtering
Implementation of image processing function in C++ that receives images of any size as an input and performs the filtering with different modalities.

All modalities of filters are applied by convolution of the kernel filter and the given image. The filters implemented are: 
- Low Pass
- High Pass
- Bandpass
- Gaussian Blur
- Laplacian

## Use
** Install dependencies before executing the following steps **
- open a new terminal and enter cmake path-to-project -DEigen3_DIR=path-to-eigen-library (for example, cmake ../bonus-project -DEigen3_DIR=/library/eigen-3.4.0/)
- in the project directory, run make 
- run ./main
- you must enter the path to the image inclusing the extension (for example, ../image.png)
- select the type of filter you want to use by selecting the number associated with the filter (enter 2 for high pass filter)
- enter the size of the kernel you wish to apply (for example, 3 for 3 x 3 kernel)
The image with the filter applied will be displayed.
 
Additional Information: 
- For the case of Gaussian and Laplacian kernels, they are calculated automatically with sigma = 1. Afterwards, the resulting filtered image is displayed. 
- An image is included in the folder but feel free to test on an image or images of your own.
- The compiler optimization flag can be changed from -O0 to up to -O3 in the CMakeLists.txt file to evaluate the difference in performance.

## Dependencies
- opencv
- Eigen3
