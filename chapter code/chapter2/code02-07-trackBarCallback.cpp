//利用滑块控件动态调整颜色下采样的级数。


#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

//全局变量
const int slider_max = 64;
int slider;
Mat image;
Mat result;

void colorReduce(Mat& inputImage, Mat& outputImage, int div);
void on_trackbar(int pos, void *);

int main(int argc, char ** argv)
{
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;

	/// 读入原始图像
	image = imread(srcFileName);
	if(image.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}

    namedWindow("源图像");
    namedWindow("显示结果");

    slider = 0;
    createTrackbar("ColorReduce", "显示结果", &slider, slider_max, on_trackbar);

    imshow("源图像",image);
    imshow("显示结果",image);

    //colorReduce(image,result,64);

    //imshow("显示结果",result);
    waitKey(0);
}

void colorReduce(Mat& inputImage, Mat& outputImage, int div)
{
    outputImage = inputImage.clone();
    int rows = outputImage.rows;
    int cols = outputImage.cols*outputImage.channels();
    if (outputImage.isContinuous())
    {
        cols = cols * rows;
        rows = 1; // 1维数组
    }

    for(int i = 0; i < rows; i++)
    {
        //uchar* data = inputImage.ptr<uchar>(i);
        uchar* dataout = outputImage.ptr<uchar>(i);
        for(int j = 0; j < cols; j++)
        {
            dataout[j] = dataout[j]/div*div + div/2;
        }
    }
}

// trackbar事件的回调函数
void on_trackbar(int pos, void *)
{

    if(pos <= 0)
        result = image;
    else
        colorReduce(image, result, pos);

    imshow("显示结果", result);
}
