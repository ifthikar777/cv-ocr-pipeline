FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install C++ compiler tools, OpenCV development libraries, and Tesseract
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libopencv-dev \
    libtesseract-dev \
    tesseract-ocr-eng \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the local source files into the container image
COPY main.cpp .
COPY CMakeLists.txt .

# Compile the C++ program inside the container
RUN mkdir build && cd build && cmake .. && make

# Set default execution command
CMD ["./build/ocr_pipeline"]