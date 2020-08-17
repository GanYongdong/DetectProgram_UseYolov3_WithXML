#include<improcess.h>
#include"Authorization2.h"
#include <math.h>

/*
void imgConvert(const cv::Mat& img, float* dst){
    uchar *data = img.data;
    int h = img.rows;
    int w = img.cols;
    int c = img.channels();

    for(int k= 0; k < c; ++k){
        for(int i = 0; i < h; ++i){
            for(int j = 0; j < w; ++j){
                dst[k*w*h+i*w+j] = data[(i*w + j)*c + k]/255.;
            }
        }
    }
}
//*/

void imgConvert(const cv::Mat& img, float* dst){
    uchar *data = img.data;
    int h = img.rows;
    int w = img.cols;
    int c = img.channels();

	int numPxls=h*w;
	int doubleNumPxls=2*numPxls;
	int i, pxlLoc;
	double scaleFactor=1.0/255;

	for(i=0; i<numPxls;i++)
	{
		pxlLoc=c*i;
		dst[i] = data[pxlLoc]*scaleFactor;
		dst[i+numPxls] = data[pxlLoc+1]*scaleFactor;
		dst[i+doubleNumPxls] = data[pxlLoc+2]*scaleFactor;
	}
}

void invImgConvert(float* dst, const cv::Mat& img){
    uchar *data = img.data;
    int h = img.rows;
    int w = img.cols;
    int c = img.channels();

	int numPxls=h*w;
	int doubleNumPxls=2*numPxls;
	int i, pxlLoc;
	double scaleFactor=255;

	for(i=0; i<numPxls;i++)
	{
		pxlLoc=c*i;
		data[pxlLoc] = dst[i]*scaleFactor;
		data[pxlLoc+1] = dst[i+numPxls] *scaleFactor;
		data[pxlLoc+2] = dst[i+doubleNumPxls] *scaleFactor;
	}
}

/*
void imgResize(float *src, float* dst,int srcWidth,int srcHeight,int dstWidth,int dstHeight){
    ///*
    /*
    switch (InitTime())
    {
    case  INTERNET_FAILED:
        MessageBox("��������ʧ��!"); PostMessage(WM_CLOSE); break;
    case VALIDITY_FAILED:
        MessageBox("�ѹ���Ч��!"); PostMessage(WM_CLOSE); break;
    default:
        switch (InitAdapter())
        {
        case  ADAPTER_MATCH_FAILED:
            MessageBox("MAC��ַ��Ȩʧ��!"); PostMessage(WM_CLOSE);break;
        case ADAPTER_GET_FAILED:
            MessageBox("������Ϣ��ȡʧ��!"); PostMessage(WM_CLOSE); break;
        }
    }*/
    /*
    switch (InitTime3())
    {
    case  INTERNET_FAILED:
        //MessageBox("��������ʧ��!"); PostMessage(WM_CLOSE);
        std::cout<<"网络连接失败!"<<std::endl;
        break;
    case VALIDITY_FAILED:
        //MessageBox("�ѹ���Ч��!"); PostMessage(WM_CLOSE);
        std::cout<<"已过有效期!"<<std::endl;
        break;
    }
    //*

    int new_w = srcWidth;
    int new_h = srcHeight;
    if (((float)dstWidth/srcWidth) < ((float)dstHeight/srcHeight)) {
        new_w = dstWidth;
        new_h = (srcHeight * dstWidth)/srcWidth;
    } else {
        new_h = dstHeight;
        new_w = (srcWidth * dstHeight)/srcHeight;
    }

    float* ImgReInner;
    size_t sizeInner=new_w*new_h*3*sizeof(float);
    ImgReInner=(float*)malloc(sizeInner);
    resizeInner(src,ImgReInner,srcWidth,srcHeight,new_w,new_h);

    for(int i=0;i<dstWidth*dstHeight*3;i++){
        dst[i]=0.5;
    }

    for(int k = 0; k < 3; ++k){
        for(int y = 0; y < new_h; ++y){
            for(int x = 0; x < new_w; ++x){
                float val = ImgReInner[k*new_w*new_h+y*new_w+x];
                dst[k*dstHeight*dstWidth + ((dstHeight-new_h)/2+y)*dstWidth + (dstWidth-new_w)/2+x]=val;
            }
        }
    }
    free(ImgReInner);
}
//*/

void resizeInner(float *src, float* dst,int srcWidth,int srcHeight,int dstWidth,int dstHeight){
    float* part;
    size_t sizePa=dstWidth*srcHeight*3*sizeof(float);
    part=(float*)malloc(sizePa);

    float w_scale = (float)(srcWidth - 1) / (dstWidth - 1);
    float h_scale = (float)(srcHeight - 1) / (dstHeight - 1);

    for(int k = 0; k < 3; ++k){
        for(int r = 0; r < srcHeight; ++r){
            for(int c = 0; c < dstWidth; ++c){
                float val = 0;
                if(c == dstWidth-1 || srcWidth == 1){
                    val=src[k*srcWidth*srcHeight+r*srcWidth+srcWidth-1];
                } else {
                    float sx = c*w_scale;
                    int ix = (int) sx;
                    float dx = sx - ix;
                    val=(1 - dx) * src[k*srcWidth*srcHeight+r*srcWidth+ix] + dx * src[k*srcWidth*srcHeight+r*srcWidth+ix+1];
                }
                part[k*srcHeight*dstWidth + r*dstWidth + c]=val;
            }
        }
    }

    for(int k = 0; k < 3; ++k){
        for(int r = 0; r < dstHeight; ++r){
            float sy = r*h_scale;
            int iy = (int) sy;
            float dy = sy - iy;
            for(int c = 0; c < dstWidth; ++c){
                float val = (1-dy) * part[k*dstWidth*srcHeight+iy*dstWidth+c];
                dst[k*dstWidth*dstHeight + r*dstWidth + c]=val;
            }
            if(r == dstHeight-1 || srcHeight == 1)
                continue;
            for(int c = 0; c < dstWidth; ++c){
                float val = dy * part[k*dstWidth*srcHeight+(iy+1)*dstWidth+c];
                dst[k*dstWidth*dstHeight + r*dstWidth + c]+=val;
            }
        }
    }
    free(part);
}

float interpLinear(float *img, int width, int height, double x, double y)
{
    
//	double xfrac = (x - floor(x));
//  double yfrac = (y - floor(y));
/*
    int xLower = (int) x;
    int xUpper = (int) x + 1;
    int yLower = (int) y;
    int yUpper = (int) y + 1;
	double xfrac = (x - xLower);
    double yfrac = (y - yLower);

//*/
//	/*
    //int xLower = floor(x);
    //int xUpper = floor(x) + 1;
    //int yLower = floor(y);
    //int yUpper = floor(y) + 1;
    int xLower = (int) x;
    int xUpper = xLower + 1;
    int yLower = (int) y;
    int yUpper = yLower + 1;

	float xfrac = (x - xLower);
    float yfrac = (y - yLower);
//*/
	float valLeftUpper;
	float valRightUpper;
	float valLeftLower;
	float valRightLower;

    float valUpper, valLower;


	valLeftUpper = getPixel(img, width, height, xLower, yUpper); 
	valRightUpper = getPixel(img, width, height, xUpper,yUpper);
	valLeftLower = getPixel(img, width, height, xLower, yLower);
	valRightLower = getPixel(img, width, height, xUpper, yLower);

    valUpper = valLeftUpper*(1.0-xfrac) + valRightUpper*(xfrac);
    valLower = valLeftLower*(1.0-xfrac) + valRightLower*(xfrac);

    return valLower*(1.0-yfrac) + valUpper*(yfrac);
}

float getPixel(float *img, int width, int height, int x, int y)
{
	float pixel;

	pixel = ((x < width && y < height && x >= 0 && y >=  0) ? img[y*width+x] : 0.0 );

	return pixel;
}

void imgResize(float *source, float* dest,int oldWidth,int oldHeight,int newWidth,int newHeight)
{
    /*
    switch (InitTime())
    {
    case  INTERNET_FAILED:
        MessageBox("��������ʧ��!"); PostMessage(WM_CLOSE); break;
    case VALIDITY_FAILED:
        MessageBox("�ѹ���Ч��!"); PostMessage(WM_CLOSE); break;
    default:
        switch (InitAdapter())
        {
        case  ADAPTER_MATCH_FAILED:
            MessageBox("MAC��ַ��Ȩʧ��!"); PostMessage(WM_CLOSE);break;
        case ADAPTER_GET_FAILED:
            MessageBox("������Ϣ��ȡʧ��!"); PostMessage(WM_CLOSE); break;
        }
    }//*/
    /*
    switch (InitTime3())
    {
    case  INTERNET_FAILED:
        //MessageBox("��������ʧ��!"); PostMessage(WM_CLOSE);
        std::cout<<"网络连接失败!"<<std::endl;
        break;
    case VALIDITY_FAILED:
        //MessageBox("�ѹ���Ч��!"); PostMessage(WM_CLOSE);
        std::cout<<"已过有效期!"<<std::endl;
        break;
    }
    //*/

    int new_w = oldWidth;
    int new_h = oldHeight;
	float scaleFactor, scaleFactor1,scaleFactor2;
	/*
	scaleFactor1=(float)newWidth/oldWidth;
	scaleFactor2=(float)newHeight/oldHeight;
    if (scaleFactor1 < scaleFactor2) {
        new_w = newWidth;
		scaleFactor = scaleFactor1;
        new_h = oldHeight * scaleFactor;
    } else {
        new_h = newHeight;
		scaleFactor = scaleFactor2;
        new_w = oldWidth * scaleFactor;
    }
	//*/
	scaleFactor1=(float)oldWidth/newWidth;
	scaleFactor2=(float)oldHeight/newHeight;
    if (scaleFactor1 > scaleFactor2) {
        new_w = newWidth;
		scaleFactor = scaleFactor1;
        new_h = oldHeight / scaleFactor;
    } else {
        new_h = newHeight;
		scaleFactor = scaleFactor2;
        new_w = oldWidth / scaleFactor;
    }
    resizeBasedOnInterpLinearWithFillingNums(source, oldWidth, oldHeight, new_w, new_h, dest, newWidth,newHeight, scaleFactor);
    //resizeBasedOnNearestValueWithFillingNums(source, oldWidth, oldHeight, new_w, new_h, dest, newWidth,newHeight, scaleFactor);
}
/*
void resizeBasedOnInterpLinearWithFillingNums(float *source,int oldWidth,int oldHeight, int newWidth,int newHeight, float* dest,int filledNewWidth,int filledNewHeight, float scaleFactor)
{

	//memset(dest, 0.5, sizeof(double)*filledNewWidth*filledNewHeight);
	int x,y;
	int oldX,oldY;
	int filledNumPxls=filledNewWidth*filledNewHeight;
	int doubleFilledNumPxls=filledNumPxls+filledNumPxls;

	int oldNumPxls=oldWidth*oldHeight;
	int doubleOldNumPxls=oldNumPxls+oldNumPxls;
	int i, pxlLoc;
	/*
    for(x = 0; x < newWidth; x++){
        for(y = 0; y < newHeight; y++){
			oldX=x*scaleFactor;
			oldY=y*scaleFactor;
			dest[y*newWidth+x] = interpLinear(source, oldWidth, oldHeight, oldX,oldY);
        }
    }
	//*
	int startX=(filledNewWidth-newWidth)/2;
	int startY=(filledNewHeight-newHeight)/2;
	int endX = newWidth+startX-1;
	int endY = newHeight+startY-1;
	for(x = 0; x < filledNewWidth; x++){
		for(y = 0; y < filledNewHeight; y++){
			pxlLoc=y*filledNewWidth+x;
			if(x<startX||y<startY||x>endX||y>endY)
			{
				dest[pxlLoc] = 0.5;
				dest[pxlLoc+filledNumPxls]  = 0.5;
				dest[pxlLoc+doubleFilledNumPxls] = 0.5;
			}
			else
			{
				oldX=(x-startX)*scaleFactor;
				oldY=(y-startY)*scaleFactor;
				dest[pxlLoc] = interpLinear(source, oldWidth, oldHeight, oldX, oldY);
				dest[pxlLoc+filledNumPxls] = interpLinear(source+oldNumPxls, oldWidth, oldHeight, oldX, oldY);
				dest[pxlLoc+doubleFilledNumPxls] = interpLinear(source+doubleOldNumPxls, oldWidth, oldHeight, oldX,oldY);
			}
		}
	}
}
//*/

void resizeBasedOnInterpLinearWithFillingNums(float *source,int oldWidth,int oldHeight, int newWidth,int newHeight, float* dest,int filledNewWidth,int filledNewHeight, float scaleFactor)
{

	//memset(dest, 0.5, sizeof(double)*filledNewWidth*filledNewHeight);
	int x,y;
	int oldX,oldY;
	int filledNumPxls=filledNewWidth*filledNewHeight;
	int doubleFilledNumPxls=filledNumPxls+filledNumPxls;

	int oldNumPxls=oldWidth*oldHeight;
	int doubleOldNumPxls=oldNumPxls+oldNumPxls;
	int pxlLoc;

    int xLower;
    int xUpper;
    int yLower;
    int yUpper;

	float xfrac;
    float yfrac;
//*/
	float valLeftUpper;
	float valRightUpper;
	float valLeftLower;
	float valRightLower;

    float valUpper, valLower;

	/*
    for(x = 0; x < newWidth; x++){
        for(y = 0; y < newHeight; y++){
			oldX=x*scaleFactor;
			oldY=y*scaleFactor;
			dest[y*newWidth+x] = interpLinear(source, oldWidth, oldHeight, oldX,oldY);
        }
    }
	//*/
	int startX=(filledNewWidth-newWidth)/2;
	int startY=(filledNewHeight-newHeight)/2;
	int endX = newWidth+startX-1;
	int endY = newHeight+startY-1;
	for(x = 0; x < filledNewWidth; x++){
		for(y = 0; y < filledNewHeight; y++){
			pxlLoc=y*filledNewWidth+x;
			if(x<startX||y<startY||x>endX||y>endY)
			{
				dest[pxlLoc] = 0.5;
				dest[pxlLoc+filledNumPxls]  = 0.5;
				dest[pxlLoc+doubleFilledNumPxls] = 0.5;
			}
			else
			{
				oldX=(x-startX)*scaleFactor;
				oldY=(y-startY)*scaleFactor;

				xLower = (int) oldX;
				xUpper = xLower + 1;
				yLower = (int) oldY;
				yUpper = yLower + 1;

				xfrac = (oldX - xLower);
				yfrac = (oldY - yLower);

				valLeftUpper = ((xLower < oldWidth && yUpper < oldHeight && xLower >= 0 && yUpper >=  0) ? source[yUpper*oldWidth+xLower] : 0.0 );
				valRightUpper = ((xUpper < oldWidth && yUpper < oldHeight && xUpper >= 0 && yUpper >=  0) ? source[yUpper*oldWidth+xUpper] : 0.0 );
				valLeftLower = ((xLower < oldWidth && yLower < oldHeight && xLower >= 0 && yLower >=  0) ? source[yLower*oldWidth+xLower] : 0.0 );
				valRightLower = ((xUpper < oldWidth && yLower < oldHeight && xUpper >= 0 && yLower >=  0) ? source[yLower*oldWidth+xUpper] : 0.0 );

				valUpper = valLeftUpper*(1.0-xfrac) + valRightUpper*(xfrac);
				valLower = valLeftLower*(1.0-xfrac) + valRightLower*(xfrac);

				dest[pxlLoc] = valLower*(1.0-yfrac) + valUpper*(yfrac);

				valLeftUpper = ((xLower < oldWidth && yUpper < oldHeight && xLower >= 0 && yUpper >=  0) ? source[yUpper*oldWidth+xLower+oldNumPxls] : 0.0 );
				valRightUpper = ((xUpper < oldWidth && yUpper < oldHeight && xUpper >= 0 && yUpper >=  0) ? source[yUpper*oldWidth+xUpper+oldNumPxls] : 0.0 );
				valLeftLower = ((xLower < oldWidth && yLower < oldHeight && xLower >= 0 && yLower >=  0) ? source[yLower*oldWidth+xLower+oldNumPxls] : 0.0 );
				valRightLower = ((xUpper < oldWidth && yLower < oldHeight && xUpper >= 0 && yLower >=  0) ? source[yLower*oldWidth+xUpper+oldNumPxls] : 0.0 );

				valUpper = valLeftUpper*(1.0-xfrac) + valRightUpper*(xfrac);
				valLower = valLeftLower*(1.0-xfrac) + valRightLower*(xfrac);
				dest[pxlLoc+filledNumPxls] = valLower*(1.0-yfrac) + valUpper*(yfrac);

				valLeftUpper = ((xLower < oldWidth && yUpper < oldHeight && xLower >= 0 && yUpper >=  0) ? source[yUpper*oldWidth+xLower+doubleOldNumPxls] : 0.0 );
				valRightUpper = ((xUpper < oldWidth && yUpper < oldHeight && xUpper >= 0 && yUpper >=  0) ? source[yUpper*oldWidth+xUpper+doubleOldNumPxls] : 0.0 );
				valLeftLower = ((xLower < oldWidth && yLower < oldHeight && xLower >= 0 && yLower >=  0) ? source[yLower*oldWidth+xLower+doubleOldNumPxls] : 0.0 );
				valRightLower = ((xUpper < oldWidth && yLower < oldHeight && xUpper >= 0 && yLower >=  0) ? source[yLower*oldWidth+xUpper+doubleOldNumPxls] : 0.0 );

				valUpper = valLeftUpper*(1.0-xfrac) + valRightUpper*(xfrac);
				valLower = valLeftLower*(1.0-xfrac) + valRightLower*(xfrac);

				dest[pxlLoc+doubleFilledNumPxls] = valLower*(1.0-yfrac) + valUpper*(yfrac);
			}
		}
	}
}

void resizeBasedOnNearestValueWithFillingNums(float *source,int oldWidth,int oldHeight, int newWidth,int newHeight, float* dest,int filledNewWidth,int filledNewHeight, float scaleFactor)
{

    //memset(dest, 0.5, sizeof(double)*filledNewWidth*filledNewHeight);
    int x,y;
    int oldX,oldY;
    int filledNumPxls=filledNewWidth*filledNewHeight;
    int doubleFilledNumPxls=filledNumPxls+filledNumPxls;

    int oldNumPxls=oldWidth*oldHeight;
    int doubleOldNumPxls=oldNumPxls+oldNumPxls;
    int pxlLoc;

    int xLower;
    int xUpper;
    int yLower;
    int yUpper;
    int xNearest, yNearest;

    float xfrac;
    float yfrac;
//*/
    float valLeftUpper;
    float valRightUpper;
    float valLeftLower;
    float valRightLower;

    float valUpper, valLower;

    /*
    for(x = 0; x < newWidth; x++){
        for(y = 0; y < newHeight; y++){
            oldX=x*scaleFactor;
            oldY=y*scaleFactor;
            dest[y*newWidth+x] = interpLinear(source, oldWidth, oldHeight, oldX,oldY);
        }
    }
    //*/
    int startX=(filledNewWidth-newWidth)/2;
    int startY=(filledNewHeight-newHeight)/2;
    int endX = newWidth+startX-1;
    int endY = newHeight+startY-1;
    for(x = 0; x < filledNewWidth; x++){
        for(y = 0; y < filledNewHeight; y++){
            pxlLoc=y*filledNewWidth+x;
            if(x<startX||y<startY||x>endX||y>endY)
            {
                dest[pxlLoc] = 0.5;
                dest[pxlLoc+filledNumPxls]  = 0.5;
                dest[pxlLoc+doubleFilledNumPxls] = 0.5;
            }
            else
            {
                oldX=(x-startX)*scaleFactor;
                oldY=(y-startY)*scaleFactor;

                xLower = (int) oldX;
                xUpper = xLower + 1;
                yLower = (int) oldY;
                yUpper = yLower + 1;

                xNearest = (oldX- xLower< xUpper-oldX ) ? xLower : xUpper;
                yNearest = (oldY- yLower< yUpper-oldY ) ? yLower : yUpper;

                dest[pxlLoc] = (xNearest < oldWidth && yNearest < oldHeight && xNearest >= 0 && yNearest >=  0) ? source[yNearest*oldWidth+xNearest] : 0.5;
                dest[pxlLoc+filledNumPxls] = (xNearest < oldWidth && yNearest < oldHeight && xNearest >= 0 && yNearest >=  0) ? source[yNearest*oldWidth+xNearest+oldNumPxls] : 0.5;
                dest[pxlLoc+doubleFilledNumPxls] = (xNearest < oldWidth && yNearest < oldHeight && xNearest >= 0 && yNearest >=  0) ? source[yNearest*oldWidth+xNearest+doubleOldNumPxls] : 0.5;
            }
        }
    }
}

void imgResize2(const cv::Mat& img, float* dest, int newWidth,int newHeight)
{
    uchar *source = img.data;
    int oldHeight = img.rows;
    int oldWidth = img.cols;
    int numChannels = img.channels();

    int newContentWidth = oldWidth;
    int newContentHeight = oldHeight;
	float scaleFactor, scaleFactor1,scaleFactor2;
	/*
	scaleFactor1=(float)newWidth/oldWidth;
	scaleFactor2=(float)newHeight/oldHeight;
    if (scaleFactor1 < scaleFactor2) {
        newContentWidth = newWidth;
		scaleFactor = scaleFactor1;
        newContentHeight = oldHeight * scaleFactor;
    } else {
        newContentHeight = newHeight;
		scaleFactor = scaleFactor2;
        newContentWidth = oldWidth * scaleFactor;
    }
	//*/
	scaleFactor1=(float)oldWidth/newWidth;
	scaleFactor2=(float)oldHeight/newHeight;
    if (scaleFactor1 > scaleFactor2) {
        newContentWidth = newWidth;
		scaleFactor = scaleFactor1;
        newContentHeight = oldHeight / scaleFactor;
    } else {
        newContentHeight = newHeight;
		scaleFactor = scaleFactor2;
        newContentWidth = oldWidth / scaleFactor;
    }
	//resizeBasedOnInterpLinearWithFillingNums(source, oldWidth, oldHeight, numChannels, newContentWidth, newContentHeight, dest, newWidth,newHeight, scaleFactor);	
    //resizeBasedOnNearestValueWithFillingNums(source, oldWidth, oldHeight, numChannels, newContentWidth, newContentHeight, dest, newWidth,newHeight, scaleFactor);
    resizeBasedOnNearestValueWithFillingNums2(source, oldWidth, oldHeight, numChannels, newContentWidth, newContentHeight, dest, newWidth,newHeight, scaleFactor);
}
	
void resizeBasedOnNearestValueWithFillingNums2(uchar *source, int oldWidth,int oldHeight, int numChannels, int newWidth,int newHeight, float* dest,int filledNewWidth,int filledNewHeight, float scaleFactor)
{

	//memset(dest, 0.5, sizeof(double)*filledNewWidth*filledNewHeight);
	int x,y;
	int oldX,oldY;
	int filledNumPxls=filledNewWidth*filledNewHeight;
	int doubleFilledNumPxls=filledNumPxls+filledNumPxls;

    //int oldNumPxls=oldWidth*oldHeight;
    //int doubleOldNumPxls=oldNumPxls+oldNumPxls;
	int pxlLoc, nearestPxlLocInSrcImg;
	float pxlValNrmFactor=1.0/255;

    int xLower;
    int xUpper;
    int yLower;
    int yUpper;
	int xNearest, yNearest;

    //float xfrac;
    //float yfrac;
//*/
    //float valLeftUpper;
    //float valRightUpper;
    //float valLeftLower;
    //float valRightLower;

    //float valUpper, valLower;

	/*
    for(x = 0; x < newWidth; x++){
        for(y = 0; y < newHeight; y++){
			oldX=x*scaleFactor;
			oldY=y*scaleFactor;
			dest[y*newWidth+x] = interpLinear(source, oldWidth, oldHeight, oldX,oldY);
        }
    }
	//*/
	int startX=(filledNewWidth-newWidth)/2;
	int startY=(filledNewHeight-newHeight)/2;
	int endX = newWidth+startX-1;
	int endY = newHeight+startY-1;
	for(x = 0; x < filledNewWidth; x++){
		for(y = 0; y < filledNewHeight; y++){
			pxlLoc=y*filledNewWidth+x;
			if(x<startX||y<startY||x>endX||y>endY)
			{
				dest[pxlLoc] = 0.5;
				dest[pxlLoc+filledNumPxls]  = 0.5;
				dest[pxlLoc+doubleFilledNumPxls] = 0.5;
			}
			else
			{
				oldX=(x-startX)*scaleFactor;
				oldY=(y-startY)*scaleFactor;

				xLower = (int) oldX;
				xUpper = xLower + 1;
				yLower = (int) oldY;
				yUpper = yLower + 1;

				xNearest = (oldX- xLower< xUpper-oldX ) ? xLower : xUpper;
				yNearest = (oldY- yLower< yUpper-oldY ) ? yLower : yUpper;

				nearestPxlLocInSrcImg = numChannels*(yNearest*oldWidth+xNearest);
				dest[pxlLoc] = (xNearest < oldWidth && yNearest < oldHeight && xNearest >= 0 && yNearest >=  0) ? source[nearestPxlLocInSrcImg+2]*pxlValNrmFactor : 0.5;
				dest[pxlLoc+filledNumPxls] = (xNearest < oldWidth && yNearest < oldHeight && xNearest >= 0 && yNearest >=  0) ? source[nearestPxlLocInSrcImg+1]*pxlValNrmFactor : 0.5;
				dest[pxlLoc+doubleFilledNumPxls] = (xNearest < oldWidth && yNearest < oldHeight && xNearest >= 0 && yNearest >=  0) ? source[nearestPxlLocInSrcImg]*pxlValNrmFactor : 0.5;
			}
		}
	}
}

void RGB2YUV( unsigned char * scr, int width, int height, unsigned char * Y, 
			 unsigned char *U, unsigned char *V )
{
	int i;

	for( i = 0; i < width * height; i = i + 1 )
	{
		Y[ i ] = (unsigned char)(0.299 * scr[ i * 3 ] + 0.587 * scr[ i * 3 +1 ] + 0.114 * scr[ i * 3 + 2 ]);
		U[ i ] = (unsigned char)(0.596 * scr[ i * 3 ] - 0.274 * scr[ i * 3 +1 ] - 0.322 * scr[ i * 3 + 2 ]);
		V[ i ] = (unsigned char)(0.211 * scr[ i * 3 ] - 0.523 * scr[ i * 3 +1 ] + 0.312 * scr[ i * 3 + 2 ]);
//		U[ i ] = (unsigned char)(-0.418 * scr[ i * 3 ] - 0.289 * scr[ i * 3 +1 ] - 0.437 * scr[ i * 3 + 2 ]);
//		V[ i ] = (unsigned char)(0.615 * scr[ i * 3 ] - 0.515 * scr[ i * 3 +1 ] - 0.100 * scr[ i * 3 + 2 ]);
	}
}
