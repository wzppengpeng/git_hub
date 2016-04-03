#ifndef _IMAGE_PREDICTOR_H_
#define _IMAGE_PREDICTOR_H_

#include <opencv2/opencv.hpp>
#include <caffe/caffe.hpp>
#include <string>
#include <memory>

#include "result.h"

namespace badboy
{
class ImagePredictor
{
public:
	ImagePredictor(const std::string& net_file, const std::string& train_val_file);
	cv::Mat ReadImageToMat(const std::string& imagePath);
	Result predict(const cv::Mat& image, const std::string& mean_file);
	Result predict(const int height, const int width, const unsigned char* data, const std::string& mean_file);

private:
	std::shared_ptr<caffe::Net<float>> caffe_test_net;
	void ConvertMatToBlob(const cv::Mat& image, caffe::Blob<float>* blob, const std::string& mean_file);
	void ConvertMemoryImageToBlob(const int height, const int width, const unsigned char* data, caffe::Blob<float>* blob, const std::string& mean_file);
	void ReadMeanToBlob(const std::string& mean_file, caffe::Blob<float>* mean);
};
}


#endif