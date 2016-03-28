#include <vector>
#include <iostream>
#include <assert.h>

#include "image_predictor.h"

using namespace badboy;
using namespace caffe;

using cv::Mat;
using std::string;
using std::vector;



cv::Mat ImagePredictor::ReadImageToMat(const std:: string& imagePath)
{
	Mat image = cv::imread(imagePath);
	return image;
}

Result ImagePredictor::predict(cv::Mat& image, const std::string& net_file, const std::string& train_val_file, const std::string& mean_file)
{
	//get the height and width

	//turn the Mat image to Blob

	Blob<float>* blob = new Blob<float>(1, 3, image.rows, image.cols);
	ConvertMatToBlob(image, blob, mean_file);
	vector<Blob<float>*> bottom;

	bottom.push_back(blob);

	//test the caffe_net
	//Setting CPU or GPU

	Caffe::set_mode(Caffe::CPU);

	//get the net
	Net<float> caffe_test_net(net_file, caffe::TEST);
	//get trained net
	caffe_test_net.CopyTrainedLayersFrom(train_val_file);

	const vector<Blob<float>*>& result = caffe_test_net.Forward(bottom, NULL);

	//std::cout<<"the size"<<result[0]->shape(0)<<" "<<result[0]->shape(1)<<std::endl;
	//get the max class and the calue of this class
	float max = 0;
	float sum = 0;
	int max_i = 0;
	for (int i = 0; i < 10; ++i) {
		float value = result[0]->cpu_data()[i];
		sum += value;
		if (max < value) {
			max = value;
			max_i = i;
		}
	}
	max /= sum;
	Result ret;
	ret.class_num = max_i;
	ret.value = max;
	if (blob)
	{
		delete blob;
		blob = NULL;
	}
	return ret;
}

void ImagePredictor::ConvertMatToBlob(const cv::Mat &image, caffe::Blob<float> *blob, const std::string& mean_file)
{
	if (image.data == NULL || image.rows <= 0 || image.cols <= 0) {
		std::cerr << "the input is invalid";
		return;
	}
	assert(blob != NULL && blob->num() == 1 && blob->channels() == 3 &&
	       blob->height() == image.rows && blob->width() == image.cols);

	int s_height = blob->height();
	int s_width = blob->width();

	//Mat image-float* mean->Blob<float>* blob
	Blob<float>* mean = new Blob<float>();
	ReadMeanToBlob(mean_file, mean);

	const float* mean_ = mean->cpu_data();
	float* data = blob->mutable_cpu_data();
	for (int h = 0; h < s_height; ++h) {
		const uchar* ptr = image.ptr<uchar>(h);
		int imageIndex = 0;
		for (int w = 0; w < s_width; ++w) {
			for (int c = 0; c < 3; ++c) {
				int topIndex = (c * s_height + h) * s_width + w;
				float pixel = static_cast<float>(ptr[imageIndex ++]);
				int meanIndex = topIndex;
				data[topIndex] = pixel - mean_[meanIndex];
			}
		}
	}
	if (mean)
	{
		delete mean;
		mean = NULL;
	}
}

void ImagePredictor::ReadMeanToBlob(const std::string& mean_file, caffe::Blob<float> *mean)
{
	BlobProto blobProto;
	ReadProtoFromBinaryFileOrDie(mean_file.c_str(), &blobProto);
	// mean = new Blob<float>();
	mean->FromProto(blobProto);
}