#include "badboy.h"
#include "recognizer.h"
#include "Config.h"
#include <assert.h>

using namespace badboy;
using std::string;

Badboy::Badboy()
	: recognizer(new Recognizer())
{

}

void Badboy::initialize(const std::string& config_file)
{
	recognizer->initialize(config_file);
}

Result Badboy::testImage()
{
	return recognizer->testImage();
}

Result Badboy::testImage(const cv::Mat& image)
{
	return recognizer->testImage(image);
}

Result Badboy::testImage(const int height, const int width, const unsigned char* data)
{
	return recognizer->testImage(height, width, data);
}

Recognizer::Recognizer()
	: isInitialized(false),
	  image_predictor(NULL),
	  m_imagePath(""),
	  m_net_file(""),
	  m_train_val_file(""),
	  m_mean_file("")

{
}

Recognizer::~Recognizer()
{
	unInitialize();
}


Result Recognizer::testImage()
{
	assert(isInitialized == true);
	cv::Mat image = image_predictor->ReadImageToMat(m_imagePath);
	return image_predictor->predict(image, m_mean_file);
}

Result Recognizer::testImage(const cv::Mat& image)
{
	assert(isInitialized == true);
	return image_predictor->predict(image, m_mean_file);
}

Result Recognizer::testImage(const int height, const int width, const unsigned char* data)
{
	assert(isInitialized == true);
	return image_predictor->predict(height, width, data, m_mean_file);
}

void Recognizer::initialize(const std::string& config_file)
{
	if (isInitialized)
	{
		unInitialize();
	}

	parseConfigFile(config_file);
	image_predictor = new ImagePredictor(m_net_file, m_train_val_file);

	isInitialized = true;
}

void Recognizer::unInitialize()
{
	if (!isInitialized)
	{
		return;
	}
	//TODO:
	if (image_predictor)
	{
		delete image_predictor;
		image_predictor = NULL;
	}

	isInitialized = true;
}

void Recognizer::parseConfigFile(const std::string& config_file)
{
	Config config;
	if (config.FileExist(config_file))
	{
		config.ReadFile(config_file, "=", ";");
	}
	else
	{
		return;
	}
	m_imagePath = config.Read("image_path", m_imagePath);
	m_net_file = config.Read("net_file", m_net_file);
	m_train_val_file = config.Read("train_val_file", m_train_val_file);
	m_mean_file = config.Read("mean_file", m_mean_file);
}
