#ifndef _BAD_BOY_H_
#define _BAD_BOY_H_

#include <string>
#include <memory>

#include <opencv2/opencv.hpp>

#include "result.h"

namespace badboy
{
class Recognizer;
class Badboy
{
public:
	Badboy();
	void initialize(const std::string& config_file);
	Result testImage();
	Result testImage(const cv::Mat& image);
	Result testImage(const int height, const int width, const unsigned char* data);
private:
	std::shared_ptr<Recognizer> recognizer;
};
}

#endif