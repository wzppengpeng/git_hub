#ifndef _BAD_BOY_JNI_H
#define _BAD_BOY_JNI_H

#include "result.h"
namespace badboy
{
Result badboy_jni(int height, int width, const unsigned char* data);
}

#endif