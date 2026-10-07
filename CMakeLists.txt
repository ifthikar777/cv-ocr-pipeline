cmake_minimum_required(VERSION 3.10)
project(CV_OCR_Pipeline)

set(CMAKE_CXX_STANDARD 17)

# Locate OpenCV and Tesseract libraries in the Linux environment
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(Tesseract REQUIRED tesseract)

include_directories(${OpenCV_INCLUDE_DIRS} ${Tesseract_INCLUDE_DIRS})

# Build the executable
add_executable(ocr_pipeline main.cpp)

# Link OpenCV and Tesseract binaries
target_link_libraries(ocr_pipeline ${OpenCV_LIBS} ${Tesseract_LIBRARIES})