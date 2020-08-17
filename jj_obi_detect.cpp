#include <jj_obj_detect.h>
#include "Authorization2.h"
#include <fstream>
#include <tinyxml/tinyxml.h>

using namespace std;
using namespace cv;

struct xmlNode
{
    int id;
    char when[32];
    char who[32];
    char where[32];
};

vector<xmlNode> nodes;

//添加子节点
void addChild_int( TiXmlElement* parent , const char* node , int value)
{
    //创建节点
    TiXmlElement* xml_node = new TiXmlElement(node);
    //将int转为char
    char buf[32];
    sprintf(buf , "%d" , value);
    xml_node->LinkEndChild(new TiXmlText(buf));
    parent->LinkEndChild(xml_node);
}

void addChild_str( TiXmlElement* parent , const char* node , const char* value)
{
    //创建节点
    TiXmlElement* xml_node = new TiXmlElement(node);
    //将int转为char
    char buf[300];
    sprintf(buf , value);
    xml_node->LinkEndChild(new TiXmlText(buf));
    parent->LinkEndChild(xml_node);
}

TiXmlDocument g_xml_doc;

int MixDetector::MixDetector_CompRunTime()
{
    long curTime=time(0);
    int runTime;
    int validTime=10800;//3 hours
    //int validTime=15;//3 hours

    runTime=curTime-beginTime;

    if(runTime>validTime)
        return VALIDITY_FAILED;//返回1表示已过有效期
    else
        return 0;//返回0表示在有效期内
}

float MixDetector::get_color(int c, int x, int max)
{
    float ratio = ((float)x/max)*5;
    int i = floor(ratio);
    int j = ceil(ratio);
    ratio -= i;
    float r = (1-ratio) * colors[i][c] + ratio*colors[j][c];
    return r;
}
void MixDetector::setup()
{
    ///*
    /*
    switch (InitTime())
    {
    case  INTERNET_FAILED:
        MessageBox("网络连接失败!"); PostMessage(WM_CLOSE); break;
    case VALIDITY_FAILED:
        MessageBox("已过有效期!"); PostMessage(WM_CLOSE); break;
    default:
        switch (InitAdapter())
        {
        case  ADAPTER_MATCH_FAILED:
            MessageBox("MAC地址授权失败!"); PostMessage(WM_CLOSE);break;
        case ADAPTER_GET_FAILED:
            MessageBox("网卡信息读取失败!"); PostMessage(WM_CLOSE); break;
        }
    }*/

//    cout<<"begin~~~~~~~~~~~~~~"<<endl;
    beginTime=time(0);
    //
    /*
    switch (InitTime3())
    {
    case  INTERNET_FAILED:
        //MessageBox("网络连接失败!"); PostMessage(WM_CLOSE);
        cout<<"网络连接失败!"<<endl;
        return;
    case VALIDITY_FAILED:
        //MessageBox("已过有效期!"); PostMessage(WM_CLOSE);
        cout<<"已过有效期!"<<endl;
        return;
    }
    //*/
    cout<<"setup~~~~~~~~~~~~~~"<<endl;
    net=load_network((char*)cfgfile.c_str(),(char*)weightfile.c_str(),0);//加载网络模型
    set_batch_network(net, 1);
    ifstream classNamesFile((char*)namesfile.c_str());//标签文件coco有80类
    if (classNamesFile.is_open())
    {
        string className = "";
        while (getline(classNamesFile, className))
            classNamesVec.push_back(className);
    }
}
int MixDetector::loadImg(Mat frame, std::string folder, std::string name, std::string path)
{

    switch (MixDetector_CompRunTime())
    {
    case  INTERNET_FAILED:
        //MessageBox("网络连接失败!"); PostMessage(WM_CLOSE);
        cout<<"网络连接失败!"<<endl;
        return -1;
    case VALIDITY_FAILED:
        //MessageBox("已过有效期!"); PostMessage(WM_CLOSE);
        cout<<"已过有效期!"<<endl;
        return -1;
    }

    //TiXmlDocument xml_doc;
    g_xml_doc.Clear();
    TiXmlElement* xml_1 = new TiXmlElement("annotation");
    g_xml_doc.LinkEndChild(xml_1);

    std::string picNameJpg = name + ".jpg";
    char folder_char[100];
    char filename_char[200];
    char path_char[300];
    strcpy(folder_char, folder.c_str());
    strcpy(filename_char, picNameJpg.c_str());
    strcpy(path_char, path.c_str());

    addChild_str(xml_1, "folder", folder_char);
    addChild_str(xml_1, "filename", filename_char);
    addChild_str(xml_1, "path", path_char);

    TiXmlElement* xml_2 = new TiXmlElement("source");
    xml_1->LinkEndChild(xml_2);
    addChild_str(xml_2, "database", "Unknown");

    TiXmlElement* xml_22 = new TiXmlElement("size");
    xml_1->LinkEndChild(xml_22);
    int picWidth = frame.cols;
    int picHeight = frame.rows;
    int picDepth = frame.channels();
    addChild_int(xml_22, "width", picWidth);
    addChild_int(xml_22, "height", picHeight);
    addChild_int(xml_22, "depth", picDepth);
    addChild_int(xml_1, "segmented", 0);

    {
//    cvtColor(frame, rgbImg, cv::COLOR_BGR2RGB);
    //float* srcImg;
//    size_t srcSize=rgbImg.rows*rgbImg.cols*3*sizeof(float);
//    srcImg=(float*)malloc(srcSize);
//    imgConvert(rgbImg,srcImg);//将图像转为yolo形式
//    float* resizeImg;
//    size_t resizeSize=net->w*net->h*3*sizeof(float);
//    resizeImg=(float*)malloc(resizeSize);
//    imgResize(srcImg,resizeImg,frame.cols,frame.rows,net->w,net->h);//缩放图像
//    network_predict(net,resizeImg);//网络推理
//    int nboxes=0;
//    detection *dets=get_network_boxes(net,rgbImg.cols,rgbImg.rows,thresh,0.5,0,1,&nboxes);

    float* resizeImg;
    size_t resizeSize=net->w*net->h*3*sizeof(float);
    resizeImg=(float*)malloc(resizeSize);
    imgResize2(frame,resizeImg,net->w,net->h);//缩放图像
    network_predict(net,resizeImg);//网络推理
    int nboxes=0;
    detection *dets=get_network_boxes(net,frame.cols,frame.rows,thresh,0.5,0,1,&nboxes);

    if(nms){
        do_nms_sort(dets,nboxes,classes,nms);
    }
    boxes.clear();
    classNames.clear();

    /*
     * 0 -      person -    soldier
     * 2 -      car -       vehicles
     * 7 -      truck -     vehicles
     * 8 -      train -     barrack
     * 41 -     cup -       oildrum
    */

    for (int i = 0; i < nboxes; i++)
    {
        //if(i == 0 || i == 2 || i == 7 || i == 8 || i == 41)
        {
            bool flag=0;
            int className;
            for(int j=0;j<classes;j++)
            {
                if(dets[i].prob[j]>thresh)
                {
                    if(!flag)
                    {
                        flag=1;
                        className=j;
                    }
                }
            }
            if(flag){
                int left = (dets[i].bbox.x - dets[i].bbox.w / 2.)*frame.cols;
                int right = (dets[i].bbox.x + dets[i].bbox.w / 2.)*frame.cols;
                int top = (dets[i].bbox.y - dets[i].bbox.h / 2.)*frame.rows;
                int bot = (dets[i].bbox.y + dets[i].bbox.h / 2.)*frame.rows;

                if (left < 0)
                    left = 0;
                if (right > frame.cols - 1)
                    right = frame.cols - 1;
                if (top < 0)
                    top = 0;
                if (bot > frame.rows - 1)
                    bot = frame.rows - 1;

                Rect box(left, top, fabs(left - right), fabs(top - bot));
                boxes.push_back(box);
                classNames.push_back(className);
            }
        }
    }
    free_detections(dets, nboxes);

    for(int i=0;i<boxes.size();i++){
        int offset = classNames[i]*123457 % 80;
        float red = 255*get_color(2,offset,80);
        float green = 255*get_color(1,offset,80);
        float blue = 255*get_color(0,offset,80);

        rectangle(frame,boxes[i],Scalar(blue,green,red),2);
        //std::cout << "classNamesVec = " << classNamesVec.size() << std::endl;

        String label = String(classNamesVec[classNames[i]]);
        /*
         * 0 -      person -    soldier
         * 2 -      car -       vehicles
         * 7 -      truck -     vehicles
         * 8 -      train -     barrack
         * 41 -     cup -       oildrum
        */
//        switch (classNames[i]) {
//        case 0:
//            label = "soldier";
//            break;
//        case 2:
//            label = "vehicles";
//            break;
//        case 7:
//            label = "vehicles";
//            break;
//        case 8:
//            label = "barrack";
//            break;
//        case 41:
//            label = "oildrum";
//            break;
//        }
        int baseLine = 0;
        Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
        putText(frame, label, Point(boxes[i].x, boxes[i].y + labelSize.height),
                FONT_HERSHEY_SIMPLEX, 1, Scalar(red, blue, green),2);

        TiXmlElement* xml_23 = new TiXmlElement("object");
        xml_1->LinkEndChild(xml_23);
        addChild_str(xml_23, "name", label.c_str());
        addChild_str(xml_23, "pse", "Unspecified");
        addChild_int(xml_23, "truncated", 0);
        addChild_int(xml_23, "difficult", 0);

        TiXmlElement* xml_31 = new TiXmlElement("bndbox");
        xml_23->LinkEndChild(xml_31);
        addChild_int(xml_31, "xmin", boxes[i].tl().x);
        addChild_int(xml_31, "ymin", boxes[i].tl().y);
        addChild_int(xml_31, "xmax", boxes[i].br().x);
        addChild_int(xml_31, "ymax", boxes[i].br().y);
    }
    std::string savePath = "/home/ganyd/lishuoshi/0725/new_images_xml/" + folder + "/" + name + ".xml";
    const char * filename = &savePath[0];
    if (folder != "")
    {
        g_xml_doc.SaveFile(filename);
    }
    //if(boxes.size()>0)
    //imshow("output",frame);

    //free(srcImg);
    free(resizeImg);
    }
    return 0;
}
MixDetector::~MixDetector()
{
    free_network(net);

}
