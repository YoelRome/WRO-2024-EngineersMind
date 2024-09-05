#include <opencv2/opencv.hpp>
#include <wiringPi.h>
#include <iostream>

using namespace cv;
using namespace std;

#define GREEN_PIN 0 // GPIO pin 11 in WiringPi
#define RED_PIN 1   // GPIO pin 12 in WiringPi

Mat detectColor(const Mat& frame, Scalar lower_bound, Scalar upper_bound) {
    Mat hsv, mask, result;
    cvtColor(frame, hsv, COLOR_BGR2HSV);
    inRange(hsv, lower_bound, upper_bound, mask);
    bitwise_and(frame, frame, result, mask);
    return result;
}

bool detectContourSize(const Mat& image, int threshold) {
    Mat gray, thresh;
    vector<vector<Point>> contours;
    
    cvtColor(image, gray, COLOR_BGR2GRAY);
    threshold(gray, thresh, 0, 255, THRESH_BINARY + THRESH_OTSU);
    findContours(thresh, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    if (!contours.empty()) {
        for (const auto& cnt : contours) {
            if (contourArea(cnt) > threshold) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    if (wiringPiSetup() == -1) {
        cout << "WiringPi setup failed!" << endl;
        return -1;
    }
    
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(RED_PIN, OUTPUT);

    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "Error opening video stream" << endl;
        return -1;
    }

    while (true) {
        Mat frame, green, red;
        cap >> frame;
        if (frame.empty()) break;

        // Detect colors
        green = detectColor(frame, Scalar(40, 100, 100), Scalar(75, 255, 255));
        red = detectColor(frame, Scalar(0, 100, 100), Scalar(20, 255, 255));

        bool green_detected = detectContourSize(green, 300);
        bool red_detected = detectContourSize(red, 50);

        digitalWrite(GREEN_PIN, green_detected ? HIGH : LOW);
        digitalWrite(RED_PIN, red_detected ? HIGH : LOW);

        imshow("Green Detection", green);
        imshow("Red Detection", red);

        if (waitKey(1) == 27) {
            break;
        }
    }

    cap.release();
    destroyAllWindows();
    return 0;
}
