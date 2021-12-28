
// 通过指定输入与输出的灰度范围实现灰度线性变换。

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

bool linearGrayTransform(const Mat& srcImg, Mat& dstImg, const Vec2i& srcGrayExt, const Vec2i& dstGrayExt)
{
	/// 为了简化代码，暂时只处理无符号字符型图像
	if(srcImg.empty() || srcImg.depth() != CV_8U)
	{
		cerr << "Invalid image format!" << endl;
		return false;
	}
	
	/// 图像通道数、行数、列数
	int iChns, iRows, iCols;
	iChns = srcImg.channels();
	iRows = srcImg.rows;
	iCols = srcImg.cols * iChns;
	
	if(srcImg.isContinuous())
	{
		iCols *= iRows;
		iRows = 1;
	}

	/// 根据输入的原始和目标灰度范围计算灰度线性变换的斜率和截距
	double alpha, beta;
	alpha = double((dstGrayExt[1] - dstGrayExt[0])) / double((srcGrayExt[1] - srcGrayExt[0]));
	beta = dstGrayExt[0] - alpha * srcGrayExt[0];

	/* 可用语句srcImg.convertTo(dstImg, -1, alpha, beta);代替下面的系列语句，实现灰度线性变换 */

	/// 初始化目标图像
	dstImg = Mat::zeros(srcImg.size(), srcImg.type());

	/// 实施灰度线性变换
	int r, c;
	const uchar *p = NULL;
	uchar *q = NULL;
	for(r = 0; r < iRows; r++)
	{
		p = srcImg.ptr<uchar>(r);
		q = dstImg.ptr<uchar>(r);
		for(c = 0; c < iCols; c++)
		{
			q[c] = saturate_cast<uchar>(alpha * p[c] + beta);
		}
	}

	return true;
}

int main(int argc, char** argv)
{
	Mat srcImg, dstImg;
	
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;

	/// 读入原始图像
	srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}

	/// 输入变换前后的灰度范围
	Vec2i inGrayExt, outGrayExt;
	cout << "Enter the source gray extent[a b]: ";
	cin >> inGrayExt[0] >> inGrayExt[1];
	cout << "Enter the target gray extent[c d]: ";
	cin >> outGrayExt[0] >> outGrayExt[1];

	/// 显示原始图像
	namedWindow("Original Image");
	imshow("Original Image", srcImg);

	/// 灰度线性变换
	linearGrayTransform(srcImg, dstImg, inGrayExt, outGrayExt);

	/// 显示变换图像
	namedWindow("Result Image");
	imshow("Result Image", dstImg);

	waitKey();

	return 0;
}

