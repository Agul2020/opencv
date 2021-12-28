/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code05-03-dftFilterLowHigh.cpp
  作  者: 耿楠
  版  本: 1.0
  日  期: 2014年3月4日
  描  述: DFT变换，通过HIGHGUI库的滑动条控制滤波器半径，
          实现的可选择性频域滤波。
  其  它:
  函数列表:
    1. Mat computeDFT();DFT变换
    2. void updateMag();计算频谱
    3. void updateResult();DFT逆变换
    4. void shift();平移频谱
    5. void drawFilledCircle();绘制实心圆
    6. static void onTrackbar(); 滑动条回调函数
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      耿楠    2014/03/04     1.0     创建文件
***********************************************************/

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

// 函数原型
Mat computeDFT(Mat image);
void updateMag(Mat complexI);
void updateResult(Mat complexI);
void shift(Mat magI);
void drawFilledCircle( Mat img, int radius, Point center );
static void onTrackbar(int pos, void *param);
// 全局变量
int kernelSize = 0;
int invertType = 0;
int normalization = 0;

int main( int argc, char** argv )
{
    // 读入图像
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat img = imread(srcFileName, IMREAD_GRAYSCALE);
	if(img.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

    // 显示原始图像
    namedWindow("Orginal window", WINDOW_AUTOSIZE);
    imshow("Orginal window", img);

    // DFT计算
    Mat complexI = computeDFT(img);

    // 创建显示频谱的窗口
    namedWindow("spectrum", WINDOW_AUTOSIZE);

    // 创建滤波控制滑动条
    createTrackbar("kernelsize", "spectrum", &kernelSize,
                   img.rows < img.cols ? img.rows / 2 : img.cols / 2,
                   onTrackbar, &complexI);
    // 创建高低通滤波控制滑动条，无回调函数，只是使用你当前值
    createTrackbar("invert", "spectrum", &invertType, 1, 0);

    // 计算频谱并转换为对数坐标进行显示
    updateMag(complexI);
    // 逆DFT变换
    updateResult(complexI);

    waitKey(0);

    return 0;
}

/*************************************************
  名    称: static void onTrackbar(int pos, void * param)
  描    述: Trackbar回调函数
  调用函数: drawFilledCircle()、shift()、updateMag()
            updateResult()(不包含OpenCV函数与C++函数)
  主调函数: Trackbar滑块滑动事件
  输入参数: int pos-->滑动条当前滑块位置
  输出参数: void * param-->数据指针，void指针，
                           可以指向任何数据，此处指向图像数据
  返    回: 无
  其    它: 无
  作    者：耿楠
  日    期：2014年3月4日
*************************************************/
static void onTrackbar(int pos, void * param)
{
    // 强制类型转换，将*param转换为Mat类型
    Mat* pComplex = (Mat*)param;
    Mat complexI = *pComplex;

    // 创建掩膜图像（初始值为0）
    Mat mask = Mat::zeros(complexI.size(), CV_8UC1);
    // 创建掩膜圆形区域
    drawFilledCircle(mask, pos, Point(mask.rows / 2.0, mask.cols / 2.0));
    // 转换为32位浮点数类型
    mask.convertTo(mask, CV_32F);
    // 归一化
    normalize(mask, mask, 1.0, 0.0, NORM_MINMAX);

    // 反转掩膜
    if(invertType)
    {
        mask = Mat::ones(mask.size(), CV_32F) - mask;
    }

    // 平移
    shift(mask);

    // 构建复数掩膜
    Mat planes[] = {Mat::zeros(complexI.size(), CV_32F),
                    Mat::zeros(complexI.size(), CV_32F)};
    Mat kernelSpec;
    planes[0] = mask; // 实部
    planes[1] = mask; // 虚部
    merge(planes, 2, kernelSpec);

    Mat complexII;
    // 按像素进行频谱相乘
    mulSpectrums(complexI, kernelSpec, complexII, DFT_ROWS);

    updateMag(complexII);		// 更新频谱显示
    updateResult(complexII);	// 反变换

    return;
}

/*************************************************
  名    称: void updateResult(Mat complexI)
  描    述: 计算DFT变换的逆变换，计算结果直接在
            该函数内部通过调用imshow()显示
  调用函数: (不包含OpenCV函数与C++函数)
  主调函数: main(),onTrackbar()函数
  输入参数: Mat complexI-->DFT变换后的复数数据矩阵
  输出参数: 无
  返    回: 无
  其    它: 无
  作    者：耿楠
  日    期：2014年3月4日
*************************************************/
void updateResult(Mat complexI)
{
    Mat work;
    idft(complexI, work);

    Mat planes[] = {Mat::zeros(complexI.size(), CV_32F),
                    Mat::zeros(complexI.size(), CV_32F)};
    /// planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))
    split(work, planes);
    /// sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
    magnitude(planes[0], planes[1], work);

    /// 归一化
    normalize(work, work, 0, 1, NORM_MINMAX);
    imshow("result", work);
}

/*************************************************
  名    称: void updateMag(Mat complexI)
  描    述: 计算DFT变换的频谱，计算结果直接在
            该函数内部通过调用imshow()显示
  调用函数: shift()(不包含OpenCV函数与C++函数)
  主调函数: main(),onTrackbar()函数
  输入参数: Mat complexI-->DFT变换后的复数数据矩阵
  输出参数: 无
  返    回: 无
  其    它: 无
  作    者：耿楠
  日    期：2014年3月4日
*************************************************/
void updateMag(Mat complexI )
{
    Mat magI;
    Mat planes[] = {Mat::zeros(complexI.size(), CV_32F),
                    Mat::zeros(complexI.size(), CV_32F)};
    /// planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))
    split(complexI, planes);
    /// sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
    magnitude(planes[0], planes[1], magI);

    /// 转换为对数坐标: log(1 + magnitude)
    magI += Scalar::all(1);
    log(magI, magI);

    /// 平移（1、3象限对换，2、4象限对换）
    shift(magI);

    // 归一化到0-1区间，以显示图像
    normalize(magI, magI, 1, 0, NORM_INF);
    imshow("spectrum", magI);
}

/*************************************************
  名    称: Mat computeDFT(Mat image)
  描    述: 执行计算DFT变换
  调用函数: 无(不包含OpenCV函数与C++函数)
  主调函数: main()函数
  输入参数: Mat image-->图像数据矩阵，仅支持
                        单通道灰度图像
  输出参数: 无
  返    回: Mat-->DFT变换后的复数数据矩阵
  其    它: 无
  作    者：耿楠
  日    期：2014年3月4日
*************************************************/
Mat computeDFT(Mat image)
{
    // 参考http://opencv.itseez.com/doc/tutorials/core/discrete_fourier_transform/discrete_fourier_transform.html

    // 扩展图像，以优化计算速度
    Mat padded;
    int m = getOptimalDFTSize( image.rows );
    int n = getOptimalDFTSize( image.cols );
    // 扩展图像，扩展的像素用0填充
    copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));

    // 为实部和虚部创建空间
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    // 创建实部和虚部双通道矩阵
    merge(planes, 2, complexI);

    // 执行傅立叶变换DFT
    dft(complexI, complexI, DFT_COMPLEX_OUTPUT);

    return complexI;
}

/*************************************************
  名    称: void shift(Mat magI)
  描    述: 平移频谱
  调用函数: 无(不包含OpenCV函数与C++函数)
  主调函数: updateMag(),onTrackbar()函数
  输入参数: Mat magI-->DFT变换的频谱数据矩阵
  输出参数: 无
  返    回: 无
  其    它: 无
  作    者：耿楠
  日    期：2014年3月4日
*************************************************/
void shift(Mat magI)
{
    // 若行列偶数进行裁减
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    int cx = magI.cols / 2;
    int cy = magI.rows / 2;

    // 平移操作
    // 1 | 4      3 | 2
    // -----  ==> -----
    // 2 | 3      4 | 1
    Mat q0(magI, Rect(0, 0, cx, cy));
    Mat q1(magI, Rect(cx, 0, cx, cy));
    Mat q2(magI, Rect(0, cy, cx, cy));
    Mat q3(magI, Rect(cx, cy, cx, cy));

    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

/*************************************************
  名    称: void drawFilledCircle(Mat img, int radius, Point center)
  描    述: 绘制实心圆
  调用函数: 无(不包含OpenCV函数与C++函数)
  主调函数: onTrackbar()函数
  输入参数: Mat mag-->图像数据
            int radius-->圆的半径
            Point center-->圆心
  输出参数: 无
  返    回: 无
  其    它: 无
  作    者：耿楠
  日    期：2014年3月4日
*************************************************/
void drawFilledCircle( Mat img, int radius, Point center )
{
    int thickness = -1;
    int lineType = 8;

    circle( img,
            center,
            radius,
            Scalar(255),
            thickness,
            lineType );
}
