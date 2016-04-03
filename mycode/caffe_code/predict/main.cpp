#include <iostream>
#include <string>

#include "badboy.h"
#include "result.h"

using namespace std;

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		cerr << "the number of input parameters is invalid" << endl;
		return -1;
	}

	badboy::Badboy boy;
	boy.initialize(argv[1]);
	badboy::Result re = boy.testImage();
	cout << re.class_num << ":" << re.value << endl;
	return 0;
}