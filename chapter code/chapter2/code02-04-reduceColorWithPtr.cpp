
//利用ptr访问图像数据，实现颜色下采样。
 

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

void colorReduce(Mat& inputImage, Mat& outputImage, int div);

int main(int argc, char ** argv)
{
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;

	/// 读入原始图像
	Mat image = imread(srcFileName);
	if(image.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}


    imshow("源图像",image);
    Mat result;//用来保存结果
    result.create(image.rows,image.cols,image.type());//它的大小、类型与原图片相匹配
    double duration;
    duration = static_cast<double>(cv::getTickCount());

    colorReduce(image,result,64);

    duration = static_cast<double>(cv::getTickCount())-duration;
    duration /= cv::getTickFrequency(); // the elapsed time in m
    cout<<"time is"<<duration<<endl;

    imshow("显示结果",result);
    waitKey(0);
}

void colorReduce(Mat& inputImage, Mat& outputImage, int div)
{
    outputImage = inputImage.clone();
    int rows = outputImage.rows;
    int cols = outputImage.cols*outputImage.channels();
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
