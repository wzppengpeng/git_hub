
#include <memory>
#include <string>

#include "mr_jni_Image_jni.h"
#include "badboy.h"
#include "result.h"

using badboy::Badboy;

static std::shared_ptr<Badboy> boy = NULL;

JNIEXPORT void JNICALL Java_mr_jni_Image_1jni_initialize
(JNIEnv *, jobject)
{
	boy = (std::shared_ptr<Badboy>)new Badboy();
	const std::string config_file = "/home/wzp/Documents/mycode/caffe_code/predict/build/config.ini";
	boy->initialize(config_file);
}



JNIEXPORT void JNICALL Java_mr_jni_Image_1jni_predictImage
(JNIEnv *env, jobject obj, jobject img, jobject re)
{
	jclass cls = env->GetObjectClass(img);
	jfieldID fidHeight = env->GetFieldID(cls, "height", "I");
	jfieldID fidWidth = env->GetFieldID(cls, "width", "I");
	jfieldID fidData = env->GetFieldID(cls, "data", "[B");


	jint height = env->GetIntField(img, fidHeight);
	jint width = env->GetIntField(img, fidWidth);
	jobject dataArray = env->GetObjectField(img, fidData);
	jbyte* data = env->GetByteArrayElements((jbyteArray)dataArray, 0);

	int height_c = height;
	int width_c = width;
	unsigned char * data_ = (unsigned char*)data;
	badboy::Result ret = boy->testImage(height_c, width_c, data_);

	jclass clc_re = env->GetObjectClass(re);
	jfieldID fidLabel = env->GetFieldID(clc_re, "label", "I");
	jfieldID fidScore = env->GetFieldID(clc_re, "score", "F");

	env->SetIntField(re, fidLabel, (jint)ret.class_num);
	env->SetFloatField(re, fidScore, (jfloat)ret.value);
}