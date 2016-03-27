#ifndef _IMAGE_PREDICOT_H
#define _IMAGE_PREDICOT_H
#include <opencv2/opencv.hpp>
#include <caffe/caffe.hpp>
#include <string>

#include "result.h"

namespace badboy
{
	class ImagePredictor
	{
		public:
			cv::Mat ReadImageToMat(const std::string& imagePath);
			Result predict(cv::Mat& image,const std::string& net_file,const std::string& train_val_file,const std::string& mean_file);
		private:
			void ConvertMatToBlob(const cv::Mat &image, caffe::Blob<float> *blob,const std::string& mean_file);
			void ReadMeanToBlob(const std::string& mean_file,caffe::Blob<float> *mean);
	};
}

#endif