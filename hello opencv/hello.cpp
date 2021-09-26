#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>

using namespace std;

int main(void)
{
    char s[] = "Hi,Cpp.";
    int n = strlen(s);
    printf("%s:%d\n",s,n);

    cout << "Hello Opencv4.5.3" << endl;
    cout << cv::COLOR_RGB2BGRA << cv::COLOR_RGB2GRAY << endl;
    cv::Mat a = cv::imread("D:/1.jpg");
    cv::imshow("hello",a);
    cv::waitKey(0);
    return 0;
}