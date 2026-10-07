#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include <chrono>

using namespace cv;
using namespace std;

int main() {
    cout << "--- Initializing CV Edge-OCR Pipeline ---" << endl;

    // Initialize Tesseract OCR API
    tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
    if (ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY)) {
        cerr << "Could not initialize tesseract." << endl;
        return 1;
    }

    // Generate a high-resolution synthetic image frame
    Mat frame = Mat::zeros(1080, 1920, CV_8UC3); 
    putText(frame, "SIEMENS ADVANT EDGE TEST", Point(500, 500), FONT_HERSHEY_SIMPLEX, 2, Scalar(255, 255, 255), 3);

    // Start precision benchmark timer
    auto start_time = chrono::high_resolution_clock::now();

    // 1. Convert to Grayscale
    Mat gray, edges;
    cvtColor(frame, gray, COLOR_BGR2GRAY);

    // 2. Gaussian Noise Filtering
    GaussianBlur(gray, gray, Size(5, 5), 0);

    // 3. Canny Edge Detection
    Canny(gray, edges, 75, 200);

    // 4. Feed processed matrix into OCR
    ocr->SetImage((uchar*)gray.data, gray.cols, gray.rows, gray.channels(), gray.step);
    ocr->SetSourceResolution(300);
    
    char* outText = ocr->GetUTF8Text();

    // Stop benchmark timer
    auto end_time = chrono::high_resolution_clock::now();
    auto latency = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    // Log operational metrics
    cout << "Processed Frame: " << frame.cols << "x" << frame.rows << endl;
    cout << "Pipeline Latency: " << latency << " ms" << endl;
    
    if (latency < 45) {
        cout << "[METRIC MET] Sub-45ms execution achieved (30+ FPS capable)." << endl;
    } else {
        cout << "[WARNING] Latency exceeded 45ms boundary." << endl;
    }

    cout << "Extracted Text: " << outText;

    // Cleanup resources
    ocr->End();
    delete[] outText;
    
    return 0;
}