#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

#include "badboy.h"
#include "result.h"

using namespace std;

int main(int argc,char* argv[])
{
	if(argc!=2&&argc!=3)
	{
		cerr<<"the input parameter is wrong"<<endl;
		return -1;
	}
	if(argc==2)
	{
		badboy::Badboy boy;
		badboy::Result re=boy.testImage(argv[1]);
		cout<<re.class_num<<" "<<re.value<<endl;
	}
	else
	{
		cv::Mat image=cv::imread(argv[2]);
		badboy::Badboy boy;
		badboy::Result re=boy.testImage(argv[1],image);
		cout<<re.class_num<<" "<<re.value<<endl;
	}
	return 0;
}