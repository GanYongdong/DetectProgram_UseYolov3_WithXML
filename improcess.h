#ifndef IMPROCESS_H
#define IMPROCESS_H
#include<opencv2/opencv.hpp>

void imgConvert(const cv::Mat& img, float* dst);

//void imgResize(float* src, float* dst,int srcWidth,int srcHeight,int dstWidth,int dstHeight);
float getPixel(float *img, int width, int height, int x, int y);
float interpLinear(float *img, int width, int height, double x, double y);
void imgResize(float *source, float* dest,int oldWidth,int oldHeight,int newWidth,int newHeight);
void resizeInner(float *src, float* dst,int srcWidth,int srcHeight,int dstWidth,int dstHeight);
void resizeBasedOnInterpLinearWithFillingNums(float *source,int oldWidth,int oldHeight, int newWidth,int newHeight, float* dest,int filledNewWidth,int filledNewHeight, float scaleFactor);
void RGB2YUV( unsigned char * scr, int width, int height, unsigned char * Y, 
			 unsigned char *U, unsigned char *V );
void invImgConvert(float* dst, const cv::Mat& img);
void resizeBasedOnNearestValueWithFillingNums(float *source,int oldWidth,int oldHeight, int newWidth,int newHeight, float* dest,int filledNewWidth,int filledNewHeight, float scaleFactor);
void resizeBasedOnNearestValueWithFillingNums2(uchar *source, int oldWidth,int oldHeight, int numChannels, int newWidth,int newHeight, float* dest,int filledNewWidth,int filledNewHeight, float scaleFactor);
void imgResize2(const cv::Mat& img, float* dest, int newWidth,int newHeight);
#endif // IMPROCESS_H

