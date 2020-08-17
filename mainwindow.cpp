#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Authorization2.h"
#include <QDebug>
#include <iterator>
#include <regex>

using namespace std;
using namespace cv;

std::vector<std::string> s_split(const std::string& in, const std::string& delim)
{
    std::regex re{ delim };
    // 调用 std::vector::vector (InputIterator first, InputIterator last,const allocator_type& alloc = allocator_type())
    // 构造函数,完成字符串分割
    return std::vector<std::string> {
        std::sregex_token_iterator(in.begin(), in.end(), re, -1),
            std::sregex_token_iterator()
    };
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    detector.setup();
    int picCnt = 1;

    cv::String path = "/home/ganyd/lishuoshi/0725/new_images/*.jpg";
    vector<cv::String> fn;
    glob(path, fn, false);
    qDebug() << QString::fromStdString(fn[0]) << endl;
    int icount = 0;

    while(1)
    {
        /*static int folderNum = 5;
        if(folderNum > 5)
        {
            std::cout << "all finish" << std::endl;
            break;
        }*/

        //while(1)
        {
            char picCntChar[8];
            std::string picFolder = "all";
            /*switch (folderNum) {
            case 0:
                sprintf(picCntChar, "%03d", picCnt);
                picFolder = "10-30-canDJI_0102";
                break;
            case 1:
                sprintf(picCntChar, "%03d", picCnt);
                picFolder = "10-30-canDJI_0103";
                break;
            case 2:
                sprintf(picCntChar, "%04d", picCnt);
                picFolder = "10-30-soldier-car-can-DJI_0107";
                break;
            case 3:
                sprintf(picCntChar, "%03d", picCnt);
                picFolder = "15-30-canDJI_0105";
                break;
            case 4:
                sprintf(picCntChar, "%04d", picCnt);
                picFolder = "20-barracksDJI_0103";
                break;
            case 5:
                sprintf(picCntChar, "%04d", picCnt);
                picFolder = "20-car-soldier-barracksDJI_0102";
                break;
            }

            std::string picCntStr = picCntChar;
            std::string picName = picFolder + " " + picCntStr;
            std::string picPath = "/home/ganyd/DataSet/军交图像集/jpg/" + picFolder + "/" + picName + ".jpg";*/

            if (icount % 10 == 0) {
                qDebug() << icount << " / " << fn.size() << endl;
            }

            if (icount > fn.size()) {
                qDebug() << "finished" << endl;
                break;
            }
            std::string picPath = fn[icount++];
            std::string delimiter = "/";
            vector<string> picPathSplit = s_split(picPath, delimiter);
            std::string picNameTmp = picPathSplit.at(6);
            std::string delimiter2 = ".jpg";
            vector<string> picPathSplit2 = s_split(picNameTmp, delimiter2);
            std::string  picName = picPathSplit2.at(0);


            cv::Mat src = cv::imread(picPath);
            //cv::Mat src = cv::imread("/home/ganyd/DataSet/军交图像集/jpg/10-30-canDJI_0103");
            if(src.empty())
            {
                //std::cout << "empty, the " << folderNum + 1 << " folder is finish.\n************************************\n\n" << std::endl;
                //picCnt = 1;
                //folderNum++;
                qDebug() << "empty or finished!" << endl;
                break;
            }

            detector.loadImg(src, picFolder, picName, picPath);

            cv::resize(src, src, cv::Size(1920, 1080));
            cv::destroyAllWindows();
//            cv::imshow("src", src);
//            waitKey(0);
//            int getInput = cv::waitKey(0);
//            if(getInput == 83) //->
//            {
//                picCnt++;
//            }
//            else if(getInput == 81 && picCnt > 1) //<-
//            {
//                picCnt--;
//            }

//            picCnt++;
//            if(picCnt % 10 == 0)
//            {
//                std::cout << "No: " << folderNum + 1 << " / " << picCnt << std::endl;
//            }
        }

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
