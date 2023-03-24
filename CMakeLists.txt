cmake_minimum_required(VERSION 3.8)
project(main VERSION 0.1.0 LANGUAGES CXX)

# Find at least version 3.3 of Eigen3
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

# Find opencv package
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(main main.cpp filter.cpp image.cpp)

# Link libaries
target_link_libraries (main Eigen3::Eigen)
target_link_libraries (main ${OpenCV_LIBS})

target_compile_features(main PUBLIC cxx_std_17)
set (CMAKE_CXX_FLAGS "-O3")