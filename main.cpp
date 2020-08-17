#include "mainwindow.h"
#include <QApplication>
#include <detector.h>
#include <opencv2/opencv.hpp>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;

    Detector detectorIns;
    //cv::Mat frame = cv::imread("/home/ganyd/dataset/2.jpeg");
//    cv::Mat frame = cv::imread("/home/ganyd/dataset/目标检测取证数据集部分/2007_000027.jpg");
//    detectorIns.test_a_frame(frame);

    std::string path = "/home/ganyd/dataset/目标检测取证数据集部分";
    detectorIns.process_a_folder_continuously(path);

    return a.exec();
}
