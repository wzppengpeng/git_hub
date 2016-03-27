#ifndef _BAD_BOY_H
#define _BAD_BOY_H

#include <string>
#include <memory>
#include <opencv2/opencv.hpp>

#include "result.h"


namespace badboy
{
	class BadboyImpl;
	class Badboy
	{
		public:
			Badboy();
			Result testImage(const std::string& config_file);
			Result testImage(const std::string& config_file,const cv::Mat& image);
		private:
			std::shared_ptr<BadboyImpl> badboyImpl;
		
	};
}

#endif