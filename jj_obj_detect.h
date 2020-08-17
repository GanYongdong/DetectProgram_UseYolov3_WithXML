#ifndef JJ_OBJ_DETECT
#define JJ_OBJ_DETECT
#include<iostream>
#include<opencv2/opencv.hpp>
#include<darknet.h>
#include<improcess.h>

using namespace std;
using namespace cv;

class MixDetector
{
public:
    void setOption();
    void setup();
    int loadImg(Mat frame, std::string folder = "", std::string name = "", std::string path = "");
    ~MixDetector();
	int MixDetector_CompRunTime();//计算运行时间

private:
//    string cfgfile = "../res/detector.cfg";//读取模型文件，请自行修改相应路径
//    string weightfile = "../res/detector.weights";
//    string namesfile = "../res/coco.names";

    string cfgfile = "/home/ganyd/projects/res/目标检测取证模型文件/my_yolov3.cfg";//读取模型文件，请自行修改相应路径
    string weightfile = "/home/ganyd/projects/res/目标检测取证模型文件/my_yolov3.backup";
    string namesfile = "/home/ganyd/projects/res/目标检测取证模型文件/myData.names";

    float thresh=0.5;//参数设置
    float nms=0.35;
    int classes=4;
    network *net;
    Mat rgbImg;
    vector<string> classNamesVec;
    Rect2d bbox;
    float colors[6][3]= { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };
    float get_color(int c, int x, int max);
    long beginTime=time(0);//程序开始时间
    vector<int>classNames;
    vector<cv::Rect>boxes;
};
#endif // JJ_OBJ_DETECT

