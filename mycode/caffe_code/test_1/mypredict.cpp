
#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include "caffe/caffe.hpp"
#include "caffe/util/io.hpp"
#include "caffe/blob.hpp"
using namespace caffe;
using namespace std;
int main(int argc, char** argv) {
	
	//Caffe::set_phase(Caffe::TEST);
	//Setting CPU or GPU
	
	Caffe::set_mode(Caffe::CPU);
	
	//get the net
	Net<float> caffe_test_net(argv[1],caffe::TEST);
	//get trained net
	caffe_test_net.CopyTrainedLayersFrom(argv[2]);
	//get datum
	Datum datum;
	if (!ReadImageToDatum(argv[3], 1, 32, 32, &datum)) {
		LOG(ERROR) << "Error during file reading";
	}
	//get the blob
	Blob<float>* blob = new Blob<float>(1, datum.channels(), datum.height(), datum.width());
	//get the blobproto
	BlobProto blob_proto;
	blob_proto.set_num(1);
	blob_proto.set_channels(datum.channels());
	blob_proto.set_height(datum.height());
	blob_proto.set_width(datum.width());
	//const int data_size = datum.channels() * datum.height() * datum.width();
	int size_in_datum = std::max<int>(datum.data().size(),
	datum.float_data_size());
	for (int i = 0; i < size_in_datum; ++i) {
		blob_proto.add_data(0.);
	}
	const string& data = datum.data();
	if (data.size() != 0) {
		for (int i = 0; i < size_in_datum; ++i) {
			blob_proto.set_data(i, blob_proto.data(i) + (uint8_t)data[i]);
		}
	}
	//set data into blob
	blob->FromProto(blob_proto);
	//fill the vector
	vector<Blob<float>*> bottom;
	bottom.push_back(blob);
	float type = 0.0;
	const vector<Blob<float>*>& result = caffe_test_net.Forward(bottom, &type);
	//Here I can use the argmax layer, but for now I do a simple for :)
	float max = 0;
	float max_i = 0;
	for (int i = 0; i < 10; ++i) {
		float value = result[0]->cpu_data()[i];
		if (max < value){
			max = value;
			max_i = i;
		}
	}
	LOG(ERROR) << "max: " << max << " i " << max_i;
	return 0;
}