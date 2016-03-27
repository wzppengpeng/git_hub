#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include "badboy.h"
#include "result.h"
#include "badboy_jni.h"


namespace badboy
{
Result badboy_jni(int height, int width, const unsigned char* data)
{
	using namespace cv;
	const std::string config_file = "/home/wzp/Documents/mycode/caffe_code/badboy/build/config.ini";
	cv::Mat image(height, width, CV_8UC3, Scalar(0, 0, 0));
	size_t arrayIndex = 0;
	for (int h = 0; h < height; ++h) {
		uchar* ptr = image.ptr<uchar>(h);
		int imageIndex = 0;
		for (int w = 0; w < (int)width; ++w) {
			for (int c = 0; c < 3; ++c) {
				ptr[imageIndex++] = data[arrayIndex++];
			}
		}
	}
	badboy::Badboy boy;
	badboy::Result re = boy.testImage(config_file, image);
	return re;
}

}