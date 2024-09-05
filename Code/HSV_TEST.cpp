#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void onTrackbar(int, void*) {}

int main() {
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "Error opening video stream" << endl;
        return -1;
    }

    namedWindow("Trackbars", WINDOW_AUTOSIZE);

    int low_H = 0, low_S = 0, low_V = 0;
    int high_H = 179, high_S = 255, high_V = 255;

    createTrackbar("Low H", "Trackbars", &low_H, 179, onTrackbar);
    createTrackbar("Low S", "Trackbars", &low_S, 255, onTrackbar);
    createTrackbar("Low V", "Trackbars", &low_V, 255, onTrackbar);
    createTrackbar("High H", "Trackbars", &high_H, 179, onTrackbar);
    createTrackbar("High S", "Trackbars", &high_S, 255, onTrackbar);
    createTrackbar("High V", "Trackbars", &high_V, 255, onTrackbar);

    Mat frame, hsv, mask, result;

    while (true) {
        cap >> frame; 
        if (frame.empty()) break;

        cvtColor(frame, hsv, COLOR_BGR2HSV);

        Scalar lower_bound(low_H, low_S, low_V);
        Scalar upper_bound(high_H, high_S, high_V);

        inRange(hsv, lower_bound, upper_bound, mask);

        bitwise_and(frame, frame, result, mask);

        imshow("Original Frame", frame);
        imshow("Mask", mask);
        imshow("Result", result);

        if (waitKey(1) == 27) {
            break;
        }
    }

    cap.release();
    destroyAllWindows();

    return 0;
}
