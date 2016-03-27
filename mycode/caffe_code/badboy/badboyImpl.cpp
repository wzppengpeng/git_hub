#include "badboy.h"
#include "badboyImpl.h"
#include "image_predictor.h"
#include "Config.h"


using namespace badboy;
using std::string;

Badboy::Badboy(): badboyImpl(new BadboyImpl())
{

}

BadboyImpl::BadboyImpl()
	: m_imagePath(""),
	  m_net_file(""),
	  m_train_val_file(""),
	  m_mean_file("")
{

}

Result Badboy::testImage(const std::string& config_file)
{
	return badboyImpl->testImage(config_file);
}

Result Badboy::testImage(const std::string& config_file, const cv::Mat& image)
{
	return badboyImpl->testImage(config_file, image);
}

Result BadboyImpl::testImage(const std::string& config_file)
{
	parseConfig(config_file);
	ImagePredictor predictor;
	cv::Mat image = predictor.ReadImageToMat(m_imagePath);
	return predictor.predict(image, m_net_file, m_train_val_file, m_mean_file);
}

Result BadboyImpl::testImage(const std::string& config_file, const cv::Mat& image)
{
	parseConfig(config_file);

	if (m_imagePath != "0")
	{
		Result ret;
		ret.class_num = -1;
		ret.value = -1;
		return ret;
	}
	//the image is from memory
	ImagePredictor predictor;
	cv::Mat img = image;
	return predictor.predict(img, m_net_file, m_train_val_file, m_mean_file);
}


void BadboyImpl::parseConfig(const std::string& config_file)
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

