#ifndef _BAD_BOY_IMPL_H
#define _BAD_BOY_IMPL_H

#include <string>
#include "result.h"

namespace badboy
{
	class BadboyImpl
	{
		public:
			BadboyImpl();
			Result testImage(const std::string& config_file);
			Result testImage(const std::string& config_file,const cv::Mat& image);
		private:
			void parseConfig(const std::string& config_file);
			
			std::string m_imagePath;
			std::string m_net_file;
			std::string m_train_val_file;
			std::string m_mean_file;
			
	};
}

#endif