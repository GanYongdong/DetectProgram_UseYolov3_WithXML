#include "detector.h"
#include <QDebug>
#include <QThread>

Detector::Detector()
{

}

void Detector::test_a_frame(Mat frame)
{
    detector.setup();

    detector.loadImg(frame);

    cv::imshow("frame", frame);
    cv::waitKey(10000);

}

void Detector::process_a_folder_continuously(string dirPath)
{
    detector.setup();

    vector<cv::String> fn;
    glob(dirPath, fn, false);
    qDebug() << QString::fromStdString(fn[0]) << endl;
    int icount = 0;

    for (int i = 0; i < fn.size(); i++) {

        cv::Mat frame = cv::imread(fn[i]);
        detector.loadImg(frame);

        cv::imshow("frame", frame);
        cv::waitKey(10);

        while (cv::waitKey(0) != int('n')) {
            QThread::msleep(10);
        }
    }
}
