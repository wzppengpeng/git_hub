#ifndef _RECOGNIZER_H_
#define _RECOGNIZER_H_

#include <string>
#include <opencv2/opencv.hpp>

#include "result.h"
#include "image_predictor.h"

namespace badboy
{
class Recognizer
{
public:
	Recognizer();
	~Recognizer();
	void initialize(const std::string& config_file);
	Result testImage();
	Result testImage(const cv::Mat& image);
	Result testImage(const int height, const int width, const unsigned char* data);
private:
	bool isInitialized;
	ImagePredictor* image_predictor;
	std::string m_imagePath;
	std::string m_net_file;
	std::string m_train_val_file;
	std::string m_mean_file;


	void unInitialize();
	void parseConfigFile(const std::string& config_file);
};
}

#endif