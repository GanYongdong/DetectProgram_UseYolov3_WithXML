#ifndef DETECTOR_H
#define DETECTOR_H

#include <opencv2/opencv.hpp>
#include <QDebug>
#include <jj_obj_detect.h>

class Detector
{
public:
    Detector();

    void test_a_frame(cv::Mat frame);

    void process_a_folder_continuously(std::string dirPath);

private:
    MixDetector detector;

};

#endif // DETECTOR_H
