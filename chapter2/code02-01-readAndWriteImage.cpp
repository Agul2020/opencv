#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{
    Mat image, result;

	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	image = imread(srcFileName);
	if(image.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}

	cout << "size: " << image.size().height << " , "<< image.size().width << endl;

    namedWindow("Original Image");// 创建一个显示窗口
    imshow("Original Image", image);

    flip(image,result,1); // 将图像水平翻转 
                          // 正值表示水平翻转 0表示水平翻转 负值表示两个方向同时翻转
    namedWindow("Output Image");
    imshow("Output Image", result);
    imwrite("output.bmp", result); // 实参中的扩展名决定了保存图像所使用的编码方式

    waitKey(0);

    return 0;
}
