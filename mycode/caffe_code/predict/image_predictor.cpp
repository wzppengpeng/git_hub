#include <vector>
#include <iostream>
#include <assert.h>

#include "image_predictor.h"

using namespace badboy;
using namespace caffe;

using cv::Mat;
using std::string;
using std::vector;

ImagePredictor::ImagePredictor(const std::string& net_file, const std::string& train_val_file, const int resized_height_, const int resized_width_, const int cropped_height_, const int cropped_width_)
	: caffe_test_net(),
	  resized_height (resized_height_),
	  resized_width (resized_width_),
	  cropped_height (cropped_height_),
	  cropped_width (cropped_width_)

{
	Caffe::set_mode(Caffe::CPU);
	caffe_test_net = (std::shared_ptr<Net<float>>)new Net<float>(net_file, caffe::TEST);
	caffe_test_net->CopyTrainedLayersFrom(train_val_file);

}

cv::Mat ImagePredictor::ReadImageToMat(const std::string & imagePath)
{
	Mat image = cv::imread(imagePath);
	return image;
}

//predict the opencv::Mat result
Result ImagePredictor::predict(const cv::Mat& image, const std::string& mean_file)
{
	//turn the Mat image to Blob

	Blob<float>* blob = new Blob<float>(1, 3, cropped_height, cropped_width);
	ConvertMatToBlob(image, blob, mean_file);
	vector<Blob<float>*> bottom;

	bottom.push_back(blob);

	//test the caffe_net

	const vector<Blob<float>*>& result = caffe_test_net->Forward(bottom, NULL);

	//get the max class and the calue of this class
	float max = 0;
	float sum = 0;
	int max_i = 0;
	for (int i = 0; i < 2; ++i) {
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

//predict the memory data image result
Result ImagePredictor::predict(const int height, const int width, const unsigned char* data, const std::string& mean_file)
{
	Blob<float>* blob = new Blob<float>(1, 3, cropped_height, cropped_width);
	ConvertMemoryImageToBlob(height, width, data, blob, mean_file);
	vector<Blob<float>*> bottom;

	bottom.push_back(blob);

	//test the caffe_net

	const vector<Blob<float>*>& result = caffe_test_net->Forward(bottom, NULL);

	//get the max class and the calue of this class
	float max = 0;
	float sum = 0;
	int max_i = 0;
	for (int i = 0; i < 2; ++i) {
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

//convert the opencv::Mat to Blob
void ImagePredictor::ConvertMatToBlob(const cv::Mat& image, caffe::Blob<float>* blob, const std::string& mean_file)
{
	if (image.data == NULL || image.rows <= 0 || image.cols <= 0) {
		std::cerr << "the input is invalid";
		return;
	}
	assert(blob != NULL && blob->num() == 1 && blob->channels() == 3 &&
	       blob->height() == cropped_height && blob->width() == cropped_width);

	//resize and crop image
	//resize
	Mat resizedImage;
	cv::resize(image, resizedImage, cv::Size(resized_height, resized_width));
	//crop
	int hOffset = 0;
	int wOffset = 0;
	hOffset = (int) ((resized_height - cropped_height + 1) * 0.5f);
	wOffset = (int) ((resized_width - cropped_width + 1) * 0.5f);

	cv::Rect roi(wOffset, hOffset, cropped_width, cropped_height);
	cv::Mat croppedImage = resizedImage(roi);


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
				int meanIndex = (c * resized_height + hOffset + h) * resized_width + wOffset + w;;
				data[topIndex] = pixel - mean_[meanIndex];
			}
		}
	}
	//dispose the mean blob
	if (mean)
	{
		delete mean;
		mean = NULL;
	}
}

//comvert thre memory image data to blob
void ImagePredictor::ConvertMemoryImageToBlob(const int height, const int width, const unsigned char* data,  caffe::Blob<float>* blob, const std::string& mean_file)
{
	if (data == NULL || height <= 0 || width <= 0) {
		std::cerr << "the input is invalid";
		return;
	}
	assert(blob != NULL && blob->num() == 1 && blob->channels() == 3 &&
	       blob->height() == cropped_height && blob->width() == cropped_width);

	//turn the memory image to Cv::Mat
	cv::Mat image(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
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

	//turn Mat to Blob
	ConvertMatToBlob(image, blob, mean_file);

}

//if the picture size is not right,resize and crop the picture
// cv::Mat ImagePredictor::ResizeAndCropImage(const cv::Mat& image)
// {
// 	//image->resizedImage
// 	Mat resizedImage;
// 	cv::resize(image, resizedImage, cv::Size(resized_height, resized_width));
// 	//resizedImage->croppedImage
// 	int hOffset = 0;
// 	int wOffset = 0;
// 	hOffset = (int) ((resized_height - cropped_height + 1) * 0.5f);
// 	wOffset = (int) ((resized_width - cropped_width + 1) * 0.5f);

// 	cv::Rect roi(wOffset, hOffset, cropped_width, cropped_height);
// 	cv::Mat croppedImage = resizedImage(roi);
// 	return croppedImage;
// }

//get the mean file
void ImagePredictor::ReadMeanToBlob(const std::string& mean_file, caffe::Blob<float>* mean)
{
	BlobProto blobProto;
	ReadProtoFromBinaryFileOrDie(mean_file.c_str(), &blobProto);

	mean->FromProto(blobProto);
}