project(IMAGE)
	#find_package(OpenCV REQUIRED)
	cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)
	link_directories(/home/wzp/Documents/mycode/caffe_code/predict/build)
	
	include_directories(/usr/lib/jvm/jdk1.7.0_80/include)
	include_directories(/usr/lib/jvm/jdk1.7.0_80/include/linux)
	include_directories(/usr/include)
	include_directories(/home/wzp/Documents/mycode/caffe_code/predict)
	add_library(predictImage SHARED mr_jni_Image_jni)
	target_link_libraries(predictImage predict)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
