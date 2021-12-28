/************************************************************
  Copyright (C), 2008-2014, 西安电子科技大学出版社版权所有.
  文件名: code04-01-resize.cpp
  作  者: 宋怀波
  版  本: 1.0
  日  期: 2014-08-11
  描  述: 实现图像的比例缩放并进行显示
  其  它: 无
  函数列表:
    1. 略
  修改记录:
      <作者>  <修改时间>   <版本>   <描述>
      宋怀波   2014-08-11     1.0     建立该文件
***********************************************************/

#include <iostream>
#include <cmath>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	/// 输入图像
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat src = imread(srcFileName, 1);
	if(src.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

	/// 比例缩放
	Mat dst;    //目标图像指针
	float scale = 0.618f; //缩放倍数，此处设定为0.618倍
	Size dst_size;  //目标图像尺寸
	dst_size.width = src.cols * scale; //目标图像的宽为源图像宽的scale倍
	dst_size.height = src.rows * scale; //目标图像的高为源图像高的scale倍
	resize(src, dst, dst_size); //缩放源图像到目标图像

	/// 显示原图像
	namedWindow("src", WINDOW_AUTOSIZE);
	imshow("src", src);

    /// 显示缩放后的图像
	namedWindow("dst", WINDOW_AUTOSIZE);
	imshow("dst", dst);

	waitKey(0);

	return 0;
}
